# 图片优化
* 选择合适的图片编码方式，PNG, JPG, SVG, Base64, WebP 等。
* 图片服务器预处理，腾讯云“数据万象”功能，在url中添加参数就可以直接对图片进行缩放，降质，裁剪等功能。
  比如，图片裁剪API文档：https://cloud.tencent.com/document/product/460/36540 。
* 图片懒加载。
* 降低请求数量，大图上传cdn，小图使用base64，icon等切雪碧图。
* 图片无损压缩，比如，使用tinyPng压缩图片。