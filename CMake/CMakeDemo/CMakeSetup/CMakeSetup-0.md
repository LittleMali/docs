# 项目
本文记录从零开始如何利用CMake搭建一个项目架构。

## 总CMakeLists
我们的项目位于CMakeDemo下。
```CMake
# 目录结构
CMakeDemo
|_ build
|_ CMake
|_ Src
|_ CMakeLists.txt
|_ .gitignore
```
* CMake：存放CMake相关的配置和定义文件。
* build：编译的缓存目录，一个空dir。
* CMakeLists.txt：项目配置的入口。

这一步主要是定义工程名。
```cmake
cmake_minimum_required(VERSION 3.10)

project(LittleCMakeDemo)
```

### 平台信息定义
利用`CMAKE_MODULE_PATH`定义平台和处理器相关的宏定义，include添加到项目中使用。  
CMake定义的变量，如何导入C/C++文件，可以使用`configure_file`或者直接`add_definitions`。

这一步主要是定义处理器和系统相关的定义，如果需要的时候，可以进一步扩展到跨平台编译。
```cmake
# CMake modules
# 添加平台/处理器架构预定义
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/Modules")
include(Platform)

# 使用CMake定义的变量：CMake-->代码的转换。
# 配置文件
configure_file(CMake/PlatformConfig.h.in PlatformConfig.h)

# 宏定义另一种写法
# 等价于 #define _BIT64 1
if(PLATFORM_PROCESSOR_BIT.64b)
    add_definitions(-D_BIT64)
else()
    add_definitions(-D_BIT32)
endif()
```

## 源码
源码主要是我们的工程代码和第三库，工程代码又可以分为业务代码和公共组件，公共组件一旦开发完成一般较少修改，我们放在Common中，业务代码我们放在Src中。
```CMake
# 目录结构
CMakeDemo
|_ build
|_ CMake
|_ Common
|_ Src
|_ CMakeLists.txt
|_ .gitignore
```
* Common：公共组件目录，跨组跨部门合作时，公共文件可以放这里。
* Src：具体的业务逻辑，存在二进制。

## 下一步
这里我们举几个例子，然后挨个来实现，我们继续看下一步[源码实现](./CMakeSetup-Src.md)。
