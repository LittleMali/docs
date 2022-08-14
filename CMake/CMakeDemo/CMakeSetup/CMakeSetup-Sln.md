# CMake 新建项目

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

## 下一步
一个空的项目还是很简单的，接下来，我们[添加源码](./CMakeSetup-Src.md)。
