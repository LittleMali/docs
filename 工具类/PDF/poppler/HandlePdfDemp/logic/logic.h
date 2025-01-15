#ifndef __LOGIC_H__
#define __LOGIC_H__

#include <cairo/cairo.h>
#include <poppler/cpp/poppler-image.h>

#include <mutex>
#include <vector>

class Logic : public TxSingleton<Logic>
{
    enum : int
    {
        ERROR_SUCCESS = 0,
        ERROR_UNKNOWN = 1,
        ERROR_INVALID_PARAMS = 2,
        ERROR_ORIGINAL_PDF_NOT_EXISTS = 3,
        ERROR_REPLACE_PDF_FAILED = 4,

        ERROR_PARSE_POLICY_FAILED = 5,
        ERROR_POLICY_NOT_ENABLE = 6,

        ERROR_PDF_OPEN_FAILED = 7,
        ERROR_CREATE_SURFACE_FAILED = 8,
        ERROR_CREATE_CR_FAILED = 9,
        ERROR_CREATE_SURFACE_PAGE_FAILED = 10,
        ERROR_ADD_IMAGE_TEXT_FAILED = 11,
        ERROR_NO_QRCODE_IMG_FILE = 12,
        ERROR_LOAD_QRCODE_IMG_FAILED = 13,
        ERROR_QRCODE_ENCODE_FAILED = 14,

        ERROR_BAKUP_ORIG_PDF_FAILED = 15,
    };

    enum : int
    {
        PointPerInch = 72, // 点数
        PrinterDPI = 120,  // 常规家用和办公室打印机的dpi在300-600之间
    };

    enum : int
    {
        // 单位：px
        PageHeaderHeight = 48, // 因为页眉页脚水印字体最大是48px
        PageFooterHeight = 48,
        PageLeftMargin = 20, // 左侧空一点，这样绘制出来的效果好看
        PageRightMargin = 20,
    };

public:
    int AddWaterMark(const std::string &file, const std::string &configbase64);

private:
    bool ParseConfig(const std::string &configbase64, Config &config);
    int StartWork(const std::string &srcPdf, const std::string &dstPdf,
                  const Config &config);

    cairo_format_t ConvertImageFormat(poppler::image::format_enum pdfImageFormat);

    int AddImageAndText(cairo_t *cr, int pageWidth, int pageHeight, const Config &config);
    int AddPageQRCode(cairo_t *cr, int pageWidth, int pageHeight, const Config &config);
    int AddPageText(cairo_t *cr, int pageWidth, int pageHeight, const Config &config);
    int AddHeaderText(cairo_t *cr, int pageWidth, int pageHeight, const Config &config);
    int AddFooterText(cairo_t *cr, int pageWidth, int pageHeight, const Config &config);

    void DrawQRCodeElem(cairo_t *cr, cairo_surface_t *qrcode, int qrcodeSize, int x, int y,
                        const Config &config);
    void DrawTextElem(cairo_t *cr, int x, int y, const Config &config);
    int ManualConvertFontPixelToPoint(int fontPixel);
};

#endif