# Qt

## 源码下载
https://download.qt.io/archive/qt/

例如，若选择某个版本（https://download.qt.io/archive/qt/5.12/5.12.12/single/），则下载对应的zip或者tar.xz格式文件。
zip适用于windows平台。
tar.xz适用于linux平台。

### linux源码安装
官方安装文档
https://wiki.qt.io/Building_Qt_5_from_Git#Configuring_and_Building
https://doc.qt.io/qt-5/configure-options.html


```bash
# 文件已下载： qt-everywhere-src-5.12.12.tar.xz
# 解压文件
xz -d qt-everywhere-src-5.12.12.tar.xz
tar -vxf qt-everywhere-src-5.12.12.tar

# 配置 & 编译
# 具体参数可以看readme
cd qt-everywhere-src-5.12.12
./configure

# configure之后会提示缺少某些依赖包，需要安装一下

make
make install

# 交叉编译配置参数
./configure xplatform=linux-aarch64-gnu-g++
```

    


