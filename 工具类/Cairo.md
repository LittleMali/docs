
```c++
cairo_surface_t* imgSurface = cairo_image_surface_create_from_png(qrcodeImg.c_str());
    cairo_status_t imgStatus = cairo_surface_status(imgSurface);
    if (CAIRO_STATUS_SUCCESS != imgStatus || !imgSurface) {
        TXLOG_INFO("create qrcode surface failed, err: %d", imgStatus);
        ret = ERROR_LOAD_QRCODE_IMG_FAILED;
        return ret;
    }

    auto imgWidth = cairo_image_surface_get_width(imgSurface);

```