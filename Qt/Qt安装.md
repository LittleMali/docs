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

  例如，若选择某个版本 `https://download.qt.io/archive/qt/5.12/5.12.12/single/`，则下载对应的zip或者tar.xz格式文件。  
  zip适用于windows平台。  
  tar.xz适用于linux平台。  

## 安装环境
* 宿主机器：x86_64架构的Ubuntu 18.04。
* 目标版本：x86_64架构和aarch64架构的Qt5.12.8。  
  我们希望的Qt版本有两套，一套是x64架构，一套是aarch64架构。  
  对于x86/x64架构的版本，在宿主环境直接编译即可。  
  对于aarch64架构的版本，是要在宿主环境交叉编译Qt。  
* 官方安装文档  
  https://wiki.qt.io/Building_Qt_5_from_Git  
* qt configure参数说明  
  https://doc.qt.io/qt-5/configure-options.html
  https://blog.csdn.net/chenyong19870904/article/details/6712409
* qt xcb依赖  
  https://doc.qt.io/qt-5/linux-requirements.html

## x86/x64 linux源码安装
  x86/x64环境下的安装比较简单，重点是弄清楚configure参数。详细的安装步骤参见官方文档。这里只列几个重要说明。  
1. 安装基础依赖包。
   `sudo apt-get qt5-default`  
   `sudo apt-get install libxcb-xinerama0-dev`  
   `sudo apt-get install build-essential perl python git`  
2. 安装xcb依赖包。  
   `sudo apt-get install libxcb-xinerama0-dev `  
   `sudo apt-get install '^libxcb.*-dev' libx11-xcb-dev libglu1-mesa-dev libxrender-dev libxi-dev libxkbcommon-dev libxkbcommon-x11-dev`  
   因为ubuntu版本差异，某些依赖包的名字可能有变化，注意搜索一下对应的名字。
3. 安装WebEngine依赖包。  
   `apt-get install libssl-dev libxcursor-dev libxcomposite-dev libxdamage-dev libxrandr-dev libdbus-1-dev libfontconfig1-dev libcap-dev libxtst-dev libpulse-dev libudev-dev libpci-dev libnss3-dev libasound2-dev libxss-dev libegl1-mesa-dev gperf bison`
4. 下载qt源码。  
   `git clone -b v5.12.8 https://github.com/qt/qt5.git`  
   `git submodule update --init --recursive`
5. configure。  
   ```bash
   configure \
    -prefix /data/output \
    -opensource -confirm-license \
    -release \
    -shared \
    -c++std c++14 \
    -nomake tests -nomake examples -no-opengl \
    -skip qt3d -skip qtcanvas3d -skip qtdatavis3d  \
    -rpath \
    -xkbcommon -eventfd \
    -pkg-config \
    -qpa xcb -qt-xcb \
    2>&1|tee build.log
   ```
  参数说明：  
  * -prefix：指明qt最终安装的路径。所有编译出来的文件都会放在这个路径下。
  * -shared： 动态链接，非静态static。WebEngine不支持static。
  * -c++std c++14：支持std，支持c++14。WebEngine需要拿C++14来编译。
  * -nomake：不编译某些模块。
  * -skip：跳过某些模块，不编译。
  * rpath：支持相对路径，Qt优先在当前路径加载依赖的lib。
  * -xkbcommon：支持xcb。
  * -qpa xcb：Qt Platform Abstraction，qt支持跨平台开发的插件（libQt5XcbQpa.so）。支持这个参数才会生成so。
  * -qt-xcb：支持跨平台，会生成libxcb.so。  
  qt configure阶段会检查大量的环境依赖，我们重点关注的是qpa和xcb相关的检查。若configure成功，则会提示继续下一步make。  
```
Features used by QPA backends:
  evdev .................................. yes
  libinput ............................... no
  INTEGRITY HID .......................... no
  mtdev .................................. no
  tslib .................................. no
  xkbcommon .............................. yes
  X11 specific:
    XLib ................................. yes
    XCB Xlib ............................. yes
    EGL on X11 ........................... yes
QPA backends:
  DirectFB ............................... no
  EGLFS .................................. yes
  EGLFS details:
    EGLFS OpenWFD ........................ no
    EGLFS i.Mx6 .......................... no
    EGLFS i.Mx6 Wayland .................. no
    EGLFS RCAR ........................... no
    EGLFS EGLDevice ...................... yes
    EGLFS GBM ............................ no
    EGLFS VSP2 ........................... no
    EGLFS Mali ........................... no
    EGLFS Raspberry Pi ................... no
    EGLFS X11 ............................ yes
  LinuxFB ................................ yes
  VNC .................................... yes
  Mir client ............................. no
  XCB:
    Using system-provided XCB libraries .. yes
    XCB XKB .............................. yes
    XCB XInput ........................... yes
    Native painting (experimental) ....... yes
    GL integrations:
      GLX Plugin ......................... yes
        XCB GLX .......................... yes
      EGL-X11 Plugin ..................... yes

  Required system libraries for qpa-xcb:
    x11 .................................. yes
    libdrm ............................... yes
    xcomposite ........................... yes
    xcursor .............................. yes
    xi ................................... yes
    xtst ................................. yes

Note: Also available for Linux: linux-clang linux-icc

Note: Disabling X11 Accessibility Bridge: D-Bus or AT-SPI is missing.

Qt is now configured for building. Just run 'make'.
Once everything is built, you must run 'make install'.
Qt will be installed into '/opt/qt-5.12.10-linux-aarch64'.

Prior to reconfiguration, make sure you remove any leftovers from
the previous build.
```
6. 编译make。
   `lscpu`  
   `sudo make -jN`  
   make -jN，N表示cpu核数，多核编译，加快编译速度。笔记本一般耗时2h起步。
7. 安装make install。
   `sudo make install`  
   这一步之后，可以在前面指定的prefix底下看到全部的qt文件。  
  

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