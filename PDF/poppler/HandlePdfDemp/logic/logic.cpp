#include "logic.h"

#include <cairo/cairo-pdf.h>
#include <cairo/cairo-ps.h>
#include <cairo/cairo-svg.h>
#include <math.h>
#include <pango/pango-font.h>
#include <pango/pango-fontmap.h>
#include <pango/pango-fontset.h>
#include <pango/pangocairo.h>
#include <poppler/cpp/poppler-document.h>
#include <poppler/cpp/poppler-embedded-file.h>
#include <poppler/cpp/poppler-global.h>
#include <poppler/cpp/poppler-page-renderer.h>
#include <poppler/cpp/poppler-page-transition.h>
#include <poppler/cpp/poppler-page.h>

#include <cmath>
#include <experimental/filesystem>
#include <iostream>
#include <memory>

namespace fs = std::experimental::filesystem;

void CustomPopplerDebugHandler(const std::string &msg, void *data)
{
}

int Logic::AddWaterMark(const std::string &file, const std::string &configstring)
{
    int ret = ERROR_UNKNOWN;

    Config config;
    if (!ParseConfig(configstring, config))
    {
        ret = ERROR_PARSE_POLICY_FAILED;
        return ret;
    }

    auto srcOrigPath = file;
    auto dstPdfPath = srcOrigPath + "_temp";

    ret = StartWork(srcOrigPath, dstPdfPath, config);
    if (ret != ERROR_SUCCESS)
    {
        return ret;
    }

    std::error_code ec2;
    if (!fs::copy_file(dstPdfPath, srcOrigPath, fs::copy_options::overwrite_existing, ec2))
    {
        LOG("bak src pdf file failed, err code: %d, msg: %s", ec2.value(), ec2.message().c_str());
        ret = ERROR_REPLACE_PDF_FAILED;
        return ret;
    }

    ret = ERROR_SUCCESS;
    return ret;
}

int Logic::StartWork(const std::string &srcPdf, const std::string &dstPdf,
                     const Config &config)
{
    int ret = ERROR_UNKNOWN;

    poppler::set_debug_error_function(CustomPopplerDebugHandler, nullptr);

    bool suc = false;
    poppler::document *doc = nullptr;
    cairo_surface_t *surface = nullptr;
    cairo_t *cr = nullptr;
    cairo_status_t status = CAIRO_STATUS_SUCCESS;

    do
    {
        // 打开PDF文件
        doc = poppler::document::load_from_file(srcPdf);
        if (!doc)
        {
            ret = ERROR_PDF_OPEN_FAILED;
            break;
        }

        // 获取pdf文件的size
        std::unique_ptr<poppler::page> firstPage(doc->create_page(0));
        if (!firstPage)
        {
            break;
        }
        poppler::rectf firstPageSize = firstPage->page_rect(poppler::media_box);

        // 创建 PDF 以保存新的 PDF 文件
        // 创建一个cairo pdf图形表面。
        surface = cairo_pdf_surface_create(dstPdf.c_str(), (int)firstPageSize.width(), (int)firstPageSize.height());
        status = cairo_surface_status(surface); // 不能仅用!surface判断，不一定准确
        if (status != CAIRO_STATUS_SUCCESS || !surface)
        {
            LOG("create dst surface failed, p: %p, err: %d", surface, status);
            ret = ERROR_CREATE_SURFACE_FAILED;
            break;
        }

        // 创建一个cairo绘图上下文，关联到pdf surface。
        cr = cairo_create(surface);
        status = cairo_status(cr);
        if (status != CAIRO_STATUS_SUCCESS || !cr)
        {
            LOG("create dst surface failed, p: %p, err: %d", cr, status);
            ret = ERROR_CREATE_CR_FAILED;
            break;
        }

        int pageHandled = 0;
        int numPages = doc->pages();

        // poppler pdf页面通过renderer渲染成png，再使用cairo加载png生成pdf文件。
        // renderer渲染的时候，dpi影响生成png的清晰度（也就是图片size）。dpi越大，图片越清晰。
        // 考虑到耗时问题，dpi根据页面数量进行调整。
        int renderDpi = 192;
        if (numPages <= 10)
        {
            renderDpi = 192;
        }
        else if (numPages <= 50)
        {
            renderDpi = 168;
        }
        else if (numPages <= 100)
        {
            renderDpi = 144;
        }
        else
        {
            renderDpi = 120;
        }

        for (int i = 0; i < numPages; ++i)
        {
            std::unique_ptr<poppler::page> pdfPage(doc->create_page(i));
            if (!pdfPage)
            {
                LOG("pdf page is null, i: %d", i);
                continue;
            }

            // 注意size的使用，double类型的变量和int类型变量的使用。
            // 在cairo paint或者finish的时候如果遇到了异常，那么，一般是size没有设置好，内存读写异常了。
            // pdf page的size，renderer将pdf page转换为png的size，cairo再将png buffer读取出来的size。

            // 目前的结论：不要对double进行转换，需要什么类型就传入什么类型。例如，pdfPage->page_rect获取的是double，
            // 那么，cairo_pdf_surface_set_size设置的时候就是double，接口的形参都是匹配的，不用额外转换。

            // 获取当前页的size
            poppler::rectf pageSize = pdfPage->page_rect(poppler::media_box);
            // 为当前页设置大小。
            // 在打印的时候，可以把某些页设置为不同的size，比如，前2页是A4纸，后2页是A3纸。
            cairo_pdf_surface_set_size(surface, pageSize.width(), pageSize.height());

            // 将pdf page渲染为png image
            poppler::page_renderer renderer;
            renderer.set_render_hint(poppler::page_renderer::text_antialiasing, true); // 平滑
            renderer.set_render_hint(poppler::page_renderer::antialiasing, true);

            double dpi = renderDpi;                                                  // 使用默认dpi的值太小了，导致转换以后的image不清晰
            double scale = dpi / 72.0;                                               // 72.0是poppler渲染时默认值，我们这是计算出来了缩放比例。
            poppler::image pdfImage = renderer.render_page(pdfPage.get(), dpi, dpi); // 渲染为image，这是个内存数据。

            // auto imgName = std::string("img_") + std::to_string(i) + ".png";
            // pdfImage.save(imgName.c_str(), "png");  // 可以sava以后看转换的效果，dpi越大，图片size越大，越清晰。

            // poppler image format转换为cairo format
            cairo_format_t cairoPageFormat = ConvertImageFormat(pdfImage.format());

            int imgStride = pdfImage.bytes_per_row();
            int imgWidth = pdfImage.width();
            int imgHeight = pdfImage.height();

            // 为当前页创建一个cairo surface，注意width和height。
            cairo_surface_t *pdfPageSurface = cairo_image_surface_create_for_data(
                reinterpret_cast<unsigned char *>(pdfImage.data()), cairoPageFormat, imgWidth, imgHeight, imgStride);
            status = cairo_surface_status(pdfPageSurface);
            if (status != CAIRO_STATUS_SUCCESS || !pdfPageSurface)
            {
                LOG("create one page surface failed, page num: %d, p: %p, err: %d", i, pdfPageSurface, status);
                ret = ERROR_CREATE_SURFACE_PAGE_FAILED;
                break;
            }

            // 绘制pdf page
            cairo_save(cr);
            cairo_scale(cr, 1.0 / scale, 1.0 / scale); // 前面pdfPageSurface是一个scaled的size，我们现在要还原回去
            cairo_set_source_surface(cr, pdfPageSurface, 0, 0);
            cairo_paint(cr);
            cairo_surface_destroy(pdfPageSurface); // 释放资源！别问，就是在这里释放，否则会异常。
            cairo_restore(cr);                     // 暂存和恢复cr，还原绘制上下文

            ret = AddImageAndText(cr, (int)pageSize.width(), (int)pageSize.height(), config);
            if (ERROR_SUCCESS != ret)
            {
                LOG("add image or text failed, page num: %d", i);
                break;
            }

            // 绘制此页pdf，并将光标切到下一页
            cairo_show_page(cr);

            pageHandled = i; // 记录一下处理到第几页了。
        }

        if (pageHandled != numPages - 1)
        {
            LOG("one of page has process failed");
            break;
        }

        // 完成整个pdf文件
        cairo_surface_finish(surface);
        suc = true;

    } while (false);

    // 释放资源
    if (cr)
    {
        cairo_destroy(cr);
        cr = nullptr;
    }
    if (surface)
    {
        cairo_surface_destroy(surface);
        surface = nullptr;
    }
    if (doc)
    {
        delete doc;
        doc = nullptr;
    }

    if (suc)
    {
        ret = ERROR_SUCCESS;
    }

    return ret;
}

cairo_format_t Logic::ConvertImageFormat(poppler::image::format_enum pdfImageFormat)
{
    cairo_format_t cairoPageFormat = CAIRO_FORMAT_ARGB32;
    switch (pdfImageFormat)
    {
    case poppler::image::format_mono:
        cairoPageFormat = CAIRO_FORMAT_A1;
        break;
    case poppler::image::format_argb32:
        cairoPageFormat = CAIRO_FORMAT_ARGB32;
        break;
    case poppler::image::format_rgb24:
        cairoPageFormat = CAIRO_FORMAT_RGB24;
        break;
    case poppler::image::format_gray8:
        cairoPageFormat = CAIRO_FORMAT_A8;
        break;
    case poppler::image::format_bgr24:
        LOG("not impl for format_bgr24, we use argb32");
        cairoPageFormat = CAIRO_FORMAT_ARGB32;
        break;
    default:
        cairoPageFormat = CAIRO_FORMAT_ARGB32;
        break;
    }

    return cairoPageFormat;
}

int Logic::AddImageAndText(cairo_t *cr, int pageWidth, int pageHeight,
                           const Config &config)
{
    int ret = ERROR_UNKNOWN;

    if (config.WaterMarkType == WaterMark::WaterMarkQRCode)
    {
        ret = AddPng(cr, pageWidth, pageHeight, config);
    }
    else
    {
        ret = AddPageText(cr, pageWidth, pageHeight, config);
    }

    AddHeaderText(cr, pageWidth, pageHeight, config);

    if (ret != ERROR_SUCCESS)
    {
        return ret;
    }

    return ret;
}

int Logic::AddPng(cairo_t *cr, int pageWidth, int pageHeight, const Config &config)
{
    int ret = ERROR_UNKNOWN;

    QRcode *qrcodeInfo = nullptr;
    cairo_surface_t *qrcodeSurface = nullptr;
    cairo_t *qrcodeCr = nullptr;
    cairo_status_t status = CAIRO_STATUS_SUCCESS;

    do
    {
        // step1：生成二维码信息
        qrcodeInfo = QRcode_encodeString(config.Character.c_str(), 0, QR_ECLEVEL_H, QR_MODE_8, 1);
        if (!qrcodeInfo)
        {
            ret = ERROR_QRCODE_ENCODE_FAILED;
            break;
        }

        // step2：准备绘制二维码
        // 先创建一个surface，用于绘制。
        qrcodeSurface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, qrcodeInfo->width, qrcodeInfo->width);
        status = cairo_surface_status(qrcodeSurface);
        if (status != CAIRO_STATUS_SUCCESS || !qrcodeSurface)
        {
            ret = ERROR_CREATE_SURFACE_FAILED;
            break;
        }

        qrcodeCr = cairo_create(qrcodeSurface);
        status = cairo_status(qrcodeCr);
        if (status != CAIRO_STATUS_SUCCESS || !qrcodeSurface)
        {
            ret = ERROR_CREATE_CR_FAILED;
            break;
        }

        // 先将画布设置为透明，这可以理解为是背景.
        // 1.0表示不透明，0.0是全透明
        cairo_set_source_rgba(qrcodeCr, 0.0, 0.0, 0.0, 0.0); // 0.0全透明，所以颜色无所谓。
        cairo_paint(qrcodeCr);                               // 我们paint了一个背景色

        // 再设置二维码的颜色，这可以理解为前景
        double red = (GetRedValue(config.Color)) * 1.0 / 255;
        double green = (GetGreenValue(config.Color)) * 1.0 / 255;
        double blue = (GetBlueValue(config.Color)) * 1.0 / 255;
        double alpha = config.TransparentVal * 1.0 / 255;
        cairo_set_source_rgba(qrcodeCr, red, green, blue, alpha); // 设置颜色，准备绘制二维码

        // 逐像素绘制二维码图片
        for (int y = 0; y < qrcodeInfo->width; ++y)
        {
            for (int x = 0; x < qrcodeInfo->width; ++x)
            {
                if (qrcodeInfo->data[y * qrcodeInfo->width + x] & 1)
                {
                    cairo_rectangle(qrcodeCr, x, y, 1, 1); // 矩形size=1，是单像素点的填充
                    cairo_fill(qrcodeCr);                  // 填充矩形
                }
            }
        }

        // 至此，我们已经准备了二维码图片，保存在qrcodeSurface中，接下来，把它叠加到pdf中即可。

        // 保存为png格式的图片
        // cairo_surface_write_to_png(qrcodeSurface, "cr-qrcode.png");  // for debug

        // step3：开始绘制二维码水印
        int rowSpace = config.ImageSize + config.RowSpace;
        int columnSpace = config.ImageSize + config.ColumnSpace;

        int rowCount = ceil((pageHeight - PageHeaderHeight - PageFooterHeight) * 1.0 / rowSpace); // 有多少行
        int columnCount = ceil((pageWidth - PageLeftMargin - PageRightMargin) * 1.0 / columnSpace);

        for (int i = 0; i < rowCount; ++i)
        {
            for (int j = 0; j < columnCount; ++j)
            {
                // 计算二维码的坐标原点
                int elemX = PageLeftMargin + columnSpace * j;
                int elemY = PageHeaderHeight + rowSpace * i;

                // 我们传入原始的二维码surfacesize，在函数内部draw的时候，自行按需要进行缩放。
                DrawPng(cr, qrcodeSurface, qrcodeInfo->width, elemX, elemY, config);
            }
        }

        ret = ERROR_SUCCESS;

    } while (false);

    // 释放资源
    if (qrcodeCr)
    {
        cairo_destroy(qrcodeCr);
        qrcodeCr = nullptr;
    }
    if (qrcodeSurface)
    {
        cairo_surface_destroy(qrcodeSurface);
        qrcodeSurface = nullptr;
    }
    if (qrcodeInfo)
    {
        QRcode_free(qrcodeInfo);
        qrcodeInfo = nullptr;
    }

    return ret;
}

void Logic::DrawPng(cairo_t *cr, cairo_surface_t *qrcode, int qrcodeSize, int x, int y,
                    const Config &config)
{
    cairo_save(cr);

    // 因为需要旋转，cairo是以坐标原点（左上角）进行旋转的。
    // 所以，旋转以后y会超出坐标系范围，因此，我们需要修正y： 边长 * sin(angle)。
    double modifyX = x;
    double modifyY = y + sin(config.Angle * M_PI / 180.0) * config.ImageSize;
    cairo_translate(cr, modifyX, modifyY);                  // 偏移坐标系，画图片用这个api，跟画文本的不一样。
    cairo_rotate(cr, -(config.Angle * 1.0 * M_PI / 180.0)); // 以原点（左上角）旋转坐标系

    // 对二维码图片进行缩放。
    double scale = config.ImageSize * 1.0 / qrcodeSize; // 正常应该是缩放这么大。但是，实际效果跟预期差太远。
    scale = scale * PointPerInch / PrinterDPI;
    cairo_scale(cr, scale, scale);
    cairo_set_source_surface(cr, qrcode, 0, 0); // 在坐标系原点绘制图片，坐标系已经translate偏移过了。
    cairo_paint(cr);                            // 绘制

    cairo_restore(cr);
}

int Logic::AddPageText(cairo_t *cr, int pageWidth, int pageHeight, const Config &config)
{

    int ret = ERROR_UNKNOWN;

    int rowSpace = config.ImageSize + config.RowSpace;
    int columnSpace = config.ImageSize + config.ColumnSpace;

    int rowCount = ceil((pageHeight - PageHeaderHeight - PageFooterHeight) * 1.0 / rowSpace); // 有多少行
    int columnCount = ceil((pageWidth - PageLeftMargin - PageRightMargin) * 1.0 / columnSpace);

    for (int i = 0; i < rowCount; ++i)
    {
        for (int j = 0; j < columnCount; ++j)
        {
            // 计算坐标原点
            int elemX = PageLeftMargin + columnSpace * j;
            int elemY = PageHeaderHeight + rowSpace * i;

            DrawTextElem(cr, elemX, elemY, config);
        }
    }

    ret = ERROR_SUCCESS;

    return ret;
}

void Logic::DrawTextElem(cairo_t *cr, int x, int y, const Config &config)
{
    cairo_save(cr);

    // cairo库不支持文本居中，自动换行等设置，因此，引入了pango库配合cairo。
    PangoLayout *layout = pango_cairo_create_layout(cr);         // 创建一个布局用于绘制文本
    pango_layout_set_text(layout, config.Character.c_str(), -1); // 设置文本内容，pango能自动识别\n
    pango_layout_set_width(layout, config.ImageSize * PANGO_SCALE);
    pango_layout_set_height(layout, config.ImageSize * PANGO_SCALE);
    pango_layout_set_wrap(layout, PANGO_WRAP_WORD);
    pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);

    // 设置字体大小pt。我们使用默认字体，就不传参了。
    auto fontSizePx = std::to_string(ManualConvertFontPixelToPoint(config.Size));
    PangoFontDescription *fontDesc = pango_font_description_from_string(fontSizePx.c_str());
    pango_layout_set_font_description(layout, fontDesc);
    pango_font_description_free(fontDesc);

    double red = (GetRedValue(config.Color)) * 1.0 / 255;
    double green = (GetGreenValue(config.Color)) * 1.0 / 255;
    double blue = (GetBlueValue(config.Color)) * 1.0 / 255;
    double alpha = config.TransparentVal * 1.0 / 255;
    cairo_set_source_rgba(cr, red, green, blue, alpha); // 颜色还是要通过cairo来设置

    // 因为文本需要旋转，cairo是以坐标原点（左上角）进行旋转的。
    // 所以，旋转以后y会超出坐标系范围，因此，我们需要修正y： 边长 * sin(angle)。
    double modifyX = x;
    double modifyY = y + sin(config.Angle * M_PI / 180.0) * config.ImageSize;
    cairo_move_to(cr, modifyX, modifyY); // 移动绘制点，从这里开始绘制

    // 先move后rotate
    cairo_rotate(cr, -(config.Angle * 1.0 * M_PI / 180.0)); // 以坐标原点（左上角）进行旋转

    pango_cairo_show_layout(cr, layout); // 绘制pango layout，文本就画出来了
    g_object_unref(layout);              // 释放资源

    cairo_restore(cr);
}

int Logic::AddHeaderText(cairo_t *cr, int pageWidth, int pageHeight, const Config &config)
{
    int ret = ERROR_UNKNOWN;

    cairo_save(cr);

    // 我们使用pango库设置文本，可以设置align center，所以，我们文本框的宽度设置为页面宽度，这样自动居中。
    int frameWidth = pageWidth - PageLeftMargin - PageRightMargin;
    int frameHeight = PageHeaderHeight;
    auto textContent = config.HeaderCharacter;
    auto textFontSizePx = config.HeaderFooterSize;
    auto textColor = config.HeaderFooterColor;
    double modifyX = PageLeftMargin; // 左侧空一点，这样好看些
    double modifyY = 5;              // 距离顶部，固定值

    // cairo库不支持文本居中，自动换行等设置，因此，引入了pango库配合cairo。
    PangoLayout *layout = pango_cairo_create_layout(cr);    // 创建一个布局用于绘制文本
    pango_layout_set_text(layout, textContent.c_str(), -1); // 设置文本内容，pango能自动识别\n
    pango_layout_set_width(layout, frameWidth * PANGO_SCALE);
    pango_layout_set_height(layout, frameHeight * PANGO_SCALE);
    pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);
    pango_layout_set_ellipsize(layout, PANGO_ELLIPSIZE_END);

    // 设置字体大小pt。我们使用默认字体，就不传参了。
    auto fontSizePx = std::to_string(ManualConvertFontPixelToPoint(textFontSizePx));
    PangoFontDescription *fontDesc = pango_font_description_from_string(fontSizePx.c_str());
    pango_layout_set_font_description(layout, fontDesc);
    pango_font_description_free(fontDesc);

    double red = (GetRedValue(textColor)) * 1.0 / 255;
    double green = (GetGreenValue(textColor)) * 1.0 / 255;
    double blue = (GetBlueValue(textColor)) * 1.0 / 255;
    cairo_set_source_rgb(cr, red, green, blue); // 颜色还是要通过cairo来设置

    cairo_move_to(cr, modifyX, modifyY); // 移动绘制点，从这里开始绘制

    pango_cairo_show_layout(cr, layout); // 绘制pango layout，文本就画出来了
    g_object_unref(layout);              // 释放资源

    cairo_restore(cr);

    ret = ERROR_SUCCESS;

    return ret;
}

int Logic::ManualConvertFontPixelToPoint(int fontPixel)
{
    // 1英寸通常定义为72点（point）。
    // pt = px * (1 / dpi) * 72

    return fontPixel * (1.0 / PrinterDPI) * PointPerInch;
}
