# Qt

## 源码下载
https://download.qt.io/archive/qt/

例如，若选择某个版本（https://download.qt.io/archive/qt/5.12/5.12.12/single/），则下载对应的zip或者tar.xz格式文件。
zip适用于windows平台。
tar.xz适用于linux平台。

### linux源码安装
```bash
# 文件已下载： qt-everywhere-src-5.12.12.tar.xz
# 解压文件
xz -d qt-everywhere-src-5.12.12.tar.xz
tar -vxf qt-everywhere-src-5.12.12.tar

# 配置 & 编译
# 具体参数可以看readme
cd qt-everywhere-src-5.12.12
./configure
make
make install
```

    
PyQt5的QLabel，使用 .setStyleSheet('text-align: center;') 尝试文字居中，居然无效!!!

可使用 .setAlignment(Qt.AlignCenter) 文字上下左右居中。

