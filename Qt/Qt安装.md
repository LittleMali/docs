# Qt

本文介绍Qt源码安装。

## 源码下载
* *推荐* 通过git下载源码。  
```bash
git clone -b v5.12.8 https://github.com/qt/qt5.git
git submodule update --init --recursive
```

* 直接下载官方源码安装包。  
https://download.qt.io/archive/qt/

例如，若选择某个版本（ https://download.qt.io/archive/qt/5.12/5.12.12/single/ ），则下载对应的zip或者tar.xz格式文件。  
zip适用于windows平台。  
tar.xz适用于linux平台。  

## 安装环境
* 宿主机器：x86_64架构的Ubuntu 18.04。
* 目标版本：x86_64架构和aarch64架构的Qt5.12.8。
  我们希望的Qt版本有两套，一套是x64架构，一套是aarch64架构。  
  对于x86/x64架构的版本，在宿主环境直接编译即可。  
  对于aarch64架构的版本，是要在宿主环境交叉编译Qt。  
### linux源码安装
* 官方安装文档
https://wiki.qt.io/Building_Qt_5_from_Git

* qt configure参数说明
https://blog.csdn.net/chenyong19870904/article/details/6712409
https://doc.qt.io/qt-5/configure-options.html

* qt交叉编译
http://www.wayln.com/?p=648
  交叉编译是 在x86平台下编译arm平台的包。

* 安装交叉编译工具
  sudo apt-get install gcc-aarch64-linux-gnu
  sudo apt-get install g++-aarch64-linux-gnu
  安装完成以后，查看是否正常。
  aarch64-linux-gnu-gcc -v
  aarch64-linux-gnu-g++ -v

  官方下载
  http://releases.linaro.org/components/toolchain/binaries/latest-7/aarch64-linux-gnu/

* qt支持xcb
第三方程序依赖qt做ui，需要打包qt的lib。qt源码编译要支持xcb。
./configure -qt-xcb
添加 -qt-xcb 参数以后，configure可能还会报错，会提示缺少依赖。把文档里面的依赖全部装一遍。
https://doc.qt.io/qt-5/linux-requirements.html

apt-get install libxcb-xinerama0-dev
sudo apt-get install '^libxcb.*-dev' libx11-xcb-dev libglu1-mesa-dev libxrender-dev libxi-dev libxkbcommon-dev libxkbcommon-x11-dev


```bash
# 文件已下载： qt-everywhere-src-5.12.12.tar.xz
# 解压文件
xz -d qt-everywhere-src-5.12.12.tar.xz
tar vxf qt-everywhere-src-5.12.12.tar

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

apt-get install -y \
libfontconfig1-dev \
libfreetype6-dev \
libx11-dev \
libx11-xcb-dev \
libxext-dev \
libxfixes-dev \
libxi-dev \
libxrender-dev \
libxcb1-dev \
libxcb-glx0-dev \
libxcb-keysyms1-dev \
libxcb-image0-dev \
libxcb-shm0-dev \
libxcb-icccm4-dev \
libxcb-sync-dev \
libxcb-xfixes0-dev \
libxcb-shape0-dev \
libxcb-randr0-dev \
libxcb-render-util0-dev \
libxcb-xinerama0-dev \
libxkbcommon-dev \
libxkbcommon-x11-dev 


ubuntu arm64
https://cdimage.ubuntu.com/netboot/18.04/?_ga=2.144972276.1373732785.1649764940-131919995.1640782375