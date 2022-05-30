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
  
## aarch64下安装Qt
  aarch64架构的qt编译，如果不涉及交叉编译，那么编译过程跟上面是完全一样的，也就是说若宿主机器是arm架构linux系统，那么，安装的依赖包也全部是arm的二进制，编译qt的时候加载的依赖也都是arm架构的，最后生成的二进制也是arm的。  
  如果是要在x86架构的linux系统中，去构建arm架构的包，这就是交叉编译。交叉编译arm版本的qt，这里涉及了较多依赖，比较无解，先后尝试了以下思路。  
  说明，之后的流程都是x86_64架构下的交叉编译。  
  宿主机器：x86_64的Ubuntu 18.04。  
  目标：交叉编译aarch64架构的Qt。

1. 安装依赖包（基础包和xcb依赖）。
2. 安装交叉编译器。  
  `sudo apt-get install gcc-aarch64-linux-gnu`  
  `sudo apt-get install g++-aarch64-linux-gnu`  
  安装完成以后，查看是否正常。  
  `aarch64-linux-gnu-gcc -v`  
  `aarch64-linux-gnu-g++ -v`

  交叉编译器源码官方下载  
  http://releases.linaro.org/components/toolchain/binaries/latest-7/aarch64-linux-gnu/  
  下载解压，并将bin路径添加到环境变量。  
  `export PATH="/download_aarch64_linux_gnu_dir/bin":$PATH`

3. 下载源码。
4. 配置configure。
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
    -xplatform linux-aarch64-gnu-g++
    2>&1|tee build.log
   ```
  参数 xplatform linux-aarch64-gnu-g++ 指明交叉编译，并使用 linux-aarch64-gnu-g++ 作为交叉编译器。参考 xplatform 的用法，在 /qt-everywhere-src-5.12.8/qtbase/mkspec里面有 xplatform 参数的定义，其中 linux-aarch64-gnu-g++ 是我们所需要的。  
  在 /qt-everywhere-src-5.12.8/qtbase/mkspec/qmake.conf 里面声明了交叉编译要使用的编译器。  

5. configure失败。  
  configure会失败，报的错误如下。失败的原因主要是xcb依赖找不到。这是因为我们在宿主环境安装的依赖全部是x86架构的，但是在编译arm架构qt时，qt要加载的是arm架构的依赖。qt试图在宿主环境中去寻找arm架构的依赖包，这肯定是不行的。
```
ERROR: Feature 'pkg-config' was enabled, but the pre-condition 'tests.pkg-config' failed.
ERROR: Feature 'xkbcommon' was enabled, but the pre-condition 'libs.xkbcommon' failed.
ERROR: Feature 'xcb' was enabled, but the pre-condition 'features.thread && features.xkbcommon && libs.xcb' failed.
```
6. 指定sysroot。  
  查阅网上其他教程，编译arm版本的qt文章虽然有，但是其宿主机器实际是arm架构的Ubuntu，例如 [Ubuntu安装QT5.13.2交叉编译环境以及程序运行](http://www.wayln.com/?p=648)、[Ubuntu18.04实现Aarch64和arm32的交叉编译全步骤(Qt5.12.10源码编译)](https://blog.csdn.net/ZLOZL/article/details/117702955)。这篇文章configure能够成功，并且xcb也是全部正常，但是它的环境跟我们需求的不一致，无法使用。  
  网上树莓派的交叉编译环境中有提到sysroot，例如[交叉编译并部署QT5.12.4到树莓派](https://www.freesion.com/article/99811071925/)。  
  `-sysroot /aarch64/usr/`可以指定sysroot，sysroot的意思是 在交叉编译qt的时候，源码所加载的依赖包，不要在当前宿主系统路径下寻找，而是要是sysroot指定的路径下寻找。这也就是说，指定的sysroot里面要放一份完整的aarch64 Ubuntu的库和二进制。  
  那么，我们的问题来了，去哪里弄到一份可用的aarch64架构Ubuntu sysroot呢？答案是不知道。在树莓派的交叉的，树莓派有提供一份py脚本，可以完成sysroot文件拉取。我们自己想要aarch64 Ubuntu目前来看是无解了。  
  本人尝试过将aarch64 Ubuntu的/usr/bin， /usr/lib, /usr/local/ 等路径全部拷贝一份作为交叉编译的sysroot，但是configure失败了。  
  `ERROR: Cannot compile a minimal program. The toolchain or QMakeSpec is broken.`
7. 源码编译xcb依赖。  
  我们指定sysroot是为了解决交叉编译中arm架构依赖的问题，这里缺失的主要是xcb的依赖包，那么，我们可以尝试手动编译arm架构的xcb依赖包。参考的几个文档如下。  
  [CentOS7 环境下交叉编译（银河麒麟）](https://blog.csdn.net/lijing_sun/article/details/111285515)  
  [【ARM开发】交叉编译Qt源码之（1）编译xcb](https://blog.csdn.net/console_log/article/details/122826798)  
  [【ARM开发】交叉编译Qt源码之（4）添加xcb支持](https://blog.csdn.net/console_log/article/details/122881398)  
  [xcb官方源码](https://www.x.org/releases/individual/xcb/)  
  几篇文章的关键是下载源码，使用交叉编译器编译，再将生成的二进制拷贝到交叉编译器的lib目录下。只是，这种方法的依赖嵌套太多，一个lib会依赖另一个lib，就像套娃一样，工作量太大。

## Qt编译
  在x86环境下尝试交叉编译arm版本qt，困难不少，前面的尝试耗时数天却没什么进展。  
  我们再回溯一下交叉编译qt的目的——生成某个安装包xxx.deb。
* xxx.deb是要支持多平台多架构的。
* xxx.deb制作安装包需要使用qt，xxx软件是使用qt做界面的。
* xxx.deb安装包的制作环境是x64 Ubuntu，我们希望在一个Ubuntu机器上打包出多平台多架构的包。因此，我们引入了交叉编译的概念 -- 更准确的说，需要交叉编译的是xxx.deb而不是qt。
* 我们需要aarch64架构的qt libs以便xxx使用，用于ui显示。那么，我们能够直接拿到aarch64架构的qt libs是不是就可以了？ -- 我们可以在arm机器Ubuntu系统上直接编译一份带xcb的Qt libs。

  准确的说，我们的全流程应该是从qt开始，到xxx.deb生成。
1. aarch64架构的Ubuntu系统，安装依赖包，下载qt源码，编译带xcb插件的qt，拷贝qt二进制（qt全部文件）。
2. x86_64架构的Ubuntu系统，使用新版本的aarch64 qt。
3. 编译制作xxx.deb。  
  编译xxx.deb的时候会失败，失败的原因是qt moc头文件的无法识别。经查，我们制作deb的时候，使用的qt moc是qt/bin底下的二进制。在我们使用了自己编译的qt二进制以后，我们实际使用的了aarch64版本的qt moc文件。在x86架构下运行arm架构的moc二进制，这肯定会失败。  
  因此，我们可以试试，使用X86_64架构编译出来的qt/bin底下的二进制，用于qt moc头文件，在打包阶段我们把aarch64版本的qt libs打进安装包。  
  
  因此，我们的全流程应该是：
1. x86_64架构的Ubuntu系统，编译x64版本的qt。我们的目的仅仅是需要qt/bin下的二进制，用于qmake，qt moc等。
2. aarch64架构的Ubuntu系统，使用交叉编译器`-xplatform linux-aarch64-gnu-g++ -xkbcommon -qpa xcb -qt-xcb`编译aarch64 qt。我们的目的是需要qt/底下的其他文件（剔除掉bin）。
3. x86_64架构的Ubuntu，xxx.deb的编译环境，准备新版本的qt。  
  qt/bin -- x86_64架构的qt/bin。  
  qt/lib等 -- aarch64架构的qt/lib等。
4. 编译xxx.deb。
  这一步我们依旧会失败，这里失败的原因是找不到lib。在编译xxx.deb的过程中，我们要加载arm架构的lib。因为我们缺少sysroot，所以，可以尝试手动把aarch64 Ubuntu的lib拷贝过来，放到qt/lib底下，这样qt在加载libs时就能在当前路径下找到依赖。  
  这里会缺不少libs，要逐个拷贝。
```
/opt/gcc-linaro-5.5.0-2017.10-x86_64_aarch64-linux-gnu/bin/../lib/gcc/aarch64-linux-gnu/5.5.0/../../../../aarch64-linux-gnu/bin/ld: warning: libnss3.so, needed by /opt/qt_arm-5.12.8/lib/libQt5WebEngineCore.so.
5.12.8, not found (try using -rpath or -rpath-link)
/opt/gcc-linaro-5.5.0-2017.10-x86_64_aarch64-linux-gnu/bin/../lib/gcc/aarch64-linux-gnu/5.5.0/../../../../aarch64-linux-gnu/bin/ld: warning: libnssutil3.so, needed by /opt/qt_arm-5.12.8/lib/libQt5WebEngineCore
.so.5.12.8, not found (try using -rpath or -rpath-link)
```
5. 安装运行xxx.deb，确认qt是否正常。