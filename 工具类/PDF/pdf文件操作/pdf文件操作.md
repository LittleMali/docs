# pdf文件操作

在Ubuntu下，有几个开源库可以用于处理PDF文件，这些库提供了C++接口。以下是一些常见的选项：

* Poppler: 
  Poppler是一个用于渲染PDF的库，它是基于xpdf的。它提供了一个C++接口，可以用于提取PDF中的文本、图像等内容。  
  在Ubuntu上，你可以使用以下命令安装Poppler：  
  `sudo apt-get install libpoppler-cpp-dev` 

* PDFium:   
  PDFium是Google的开源PDF渲染库，它被用在Chrome浏览器中。它提供了一个C++接口，可以用于渲染和操作PDF文件。PDFium的安装可能比较复杂，因为它不在Ubuntu的软件库中，你可能需要从源代码编译它。

* PoDoFo: 
  PoDoFo是一个用于处理PDF的开源库，它提供了一个C++接口。你可以使用PoDoFo来创建、修改和解析PDF文件。
  在Ubuntu上，你可以使用以下命令安装PoDoFo：
  `sudo apt-get install libpodofo-dev`

## poppler库
poppler排在了第一个，就选择了poppler来处理pdf文件。

在demo代码中，我们打开一个pdf文件，逐页为其添加png或者文本。  
我们一共要引入3个库。
* popper：pdf操作库。
* cairo：图片处理库。
* pango：文本处理库。

### cairo引入
cairo是个基础库，poppler和pango都会依赖它。
* 版本：cairo-1.16.0。
* 方式：源码编译。
  
```shell
# 编译步骤
./configure
make -j6
make install

# so文件
/usr/local/lib/cairo/libcairo.so
# 头文件
/usr/local/include/cairo
```

### poppler引入


* 版本：`poppler-22.12.0`
* 方式：源码编译。
  不能使用最新版，最新版在centos7.5下编译不过。

```
# 编译步骤
mkdir build
cd build
# 配置使用cairo的位置，屏蔽test，屏蔽qt5，qt6等。
cmake -DCMAKE_PREFIX_PATH=/usr/local/lib -DBUILD_GTK_TESTS=OFF -DBUILD_QT5_TESTS=OFF -DBUILD_QT6_TESTS=OFF -DBUILD_CPP_TESTS=OFF -DBUILD_MANUAL_TESTS=OFF -DENABLE_BOOST=OFF -DENABLE_GLIB=OFF -DENABLE_GOBJECT_INTROSPECTION=OFF -DENABLE_QT5=OFF -DENABLE_QT6=OFF -DENABLE_LIBCURL=OFF ..

make -j6
make install

# make install以后
/usr/local/lib64/libpoppler.so.126.0.0
/usr/local/lib64/libpoppler-cpp.so.0.11.0
/usr/local/include/poppler
```

#### poppler坑1
采用yum install的方式安装poppler，拷贝so会有链接问题，某个api无法找到。  
看libpoppler-cpp.so的函数原型，跟本机gcc链接的原型有差异，本机试图链接的函数原型多了CXX11。  
因此，poppler采用源码编译的方式。
#### poppler坑2
cmake配置可能失败，提示缺少某些依赖库。缺少什么就安装什么，eg，在arm下缺少了：
```cmake
CMake Error at CMakeLists.txt:158 (message):
  Install libjpeg before trying to build poppler.  You can also decide to use
  the internal unmaintained DCT decoder or none at all.

# 解决
yum install libjpeg-devel
```
```cmake
-- Could NOT find openjpeg2.
CMake Error at CMakeLists.txt:264 (message):
  Install libopenjpeg2 before trying to build poppler.  You can also decide
  to use the internal unmaintained JPX decoder or none at all.

# 解决
yum install openjpeg2-devel
```
#### poppler坑3
* 现象：编译失败。
```
In file included from /home/semonma/poppler-22.12.0/poppler/SignatureInfo.cc:28:0:
/usr/include/nss3/hasht.h:48:29: error: 'PRBool' has not been declared
     void (*destroy)(void *, PRBool);
```
* 解决方案：`./poppler/SignatureInfo.h`, `./utils/pdfsig.cc`增加`#include <prtypes.h>`。
* 说明：centos7下，系统头文件未正确include。  
  参考1：https://gitlab.freedesktop.org/poppler/poppler/-/issues/618 。  
  参考2：检查头文件包含顺序：检查Poppler源代码中包含nss3头文件的顺序。确保在包含hasht.h之前包含了prtypes.h，因为prtypes.h中定义了PRBool类型。例如：
```
  #include <nss3/prtypes.h>
  #include <nss3/hasht.h>
```

## pango引入
方式：yum安装
```
Ubuntu安装：sudo apt-get install libpango1.0-dev
centos安装：sudo yum install pango-devel
```

## glib引入
pango安装的时候会自动安装glib，pango是强依赖glib的，