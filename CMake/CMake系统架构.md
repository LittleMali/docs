# CMake系统架构

如何判断系统架构，程序位数等。

## 判断系统
* `CMAKE_SYSTEM`：系统名称，比如 Linux-­2.6.22。  
* `CMAKE_SYSTEM_NAME`：不包含版本的系统名，比如 Linux。
* `CMAKE_SYSTEM_VERSION`：系统版本，比如 2.6.22。

## 判断CPU架构
* `CMAKE_SYSTEM_PROCESSOR`：目标系统的处理器名称，比如 i686。
* `CMAKE_HOST_SYSTEM_NAME`：上面那个是交叉编译的目标系统，这个是当前宿主机。
* `UNIX`：在所有的类 UNIX 平台下该值为 TRUE，包括 OS X 和 cygwin。
* `WIN32`：在所有的 win32 平台下该值为 TRUE，包括 cygwin。

交叉编译的话系统名也可以自己在xxx.cmake中设置。  
set(CMAKE_SYSTEM_NAME Linux)  
set(CMAKE_SYSTEM_PROCESSOR aarch64)  
说明：似乎在网上看到过，如果要覆盖cmake这两个字段的默认值，那么要同时设置这两个字段，否则会出现另一个字段为空的问题。

## 判断CPU位数
* `CMAKE_SIZEOF_VOID_P`：表示 void* 的大小（例如为 4 或者 8），可以使用其来判断当前构建为 32 位还是 64 位。使用 CMAKE_SIZEOF_VOID_P 是检查当前CPU是否具有32位或64位架构的唯一“真正”可移植的方法。
```cmake
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	message(STATUS "Target is 64 bits")
else()
	message(STATUS "Target is 32 bits")
endif()
```

## CMake如何识别平台架构
cmake在检测编译器的时候，用了一种很暴力的方法。可以在不运行实际代码的情况下直接知道目标平台的信息。做法是这样的。  
首先生成一个.cpp文件，包含一些平台检测的#ifdef以及架构检测的#ifdef。
```c++
/* Identify known platforms by name.  */
#if defined(__linux) || defined(__linux__) || defined(linux)
# define PLATFORM_ID "Linux"
 
#elif defined(__CYGWIN__)
# define PLATFORM_ID "Cygwin"
 
#elif defined(__MINGW32__)
# define PLATFORM_ID "MinGW"
 
#elif defined(__APPLE__)
# define PLATFORM_ID "Darwin"
 
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
# define PLATFORM_ID "Windows"
```
```c++
#if defined(_WIN32) && defined(_MSC_VER)
# if defined(_M_IA64)
#  define ARCHITECTURE_ID "IA64"
 
# elif defined(_M_X64) || defined(_M_AMD64)
#  define ARCHITECTURE_ID "x64"
 
# elif defined(_M_IX86)
#  define ARCHITECTURE_ID "X86"
 
# elif defined(_M_ARM)
#  if _M_ARM == 4
#   define ARCHITECTURE_ID "ARMV4I"
#  elif _M_ARM == 5
#   define ARCHITECTURE_ID "ARMV5I"
#  else
```
之后，定义两个char const*常量。然后把这个cpp编译成二进制。  
你以为要运行它？不是的。  
对于交叉编译的情况来说，目标平台的二进制是没法在编译平台上运行的。所以cmake用了一招更狠的，直接打开二进制文件，搜索字符串。比如在Windows x64平台上，刚才那两个常量就会被编译INFO:platform[Windows]和INFO:arch[x64]。而这样的字符串，一定存在于二进制文件里的。所以只要搜INFO:platform，后面[]里的就是平台；搜索INFO:arch，后面[ ]里的就是架构。同理，cmake里还有对编译器和编译器版本等信息的检测，都是这么来的。