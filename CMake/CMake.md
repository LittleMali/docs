# CMake

CMake是个一个开源的跨平台自动化建构系统，用来管理软件建置的程序，并不依赖于某特定编译器，并可支持多层目录、多个应用程序与多个函式库。 它用配置文件控制建构过程（build process）的方式和Unix的make相似，只是CMake的配置文件取名为CMakeLists.txt。CMake并不直接建构出最终的软件，而是产生标准的建构档（如Unix的Makefile或Windows Visual C++的projects/workspaces），然后再依一般的建构方式使用。

“CMake”这个名字是“Cross platform Make”的缩写。虽然名字中含有“make”，但是CMake和Unix上常见的make系统是分开的，而且更为高阶。它可与原生建置环境结合使用，例如：make、ninja、苹果公司的Xcode与微软的Microsoft Visual Studio。

## 教程
https://github.com/ttroy50/cmake-examples.git
https://cmake.org/cmake/help/latest/guide/tutorial/index.html

## 学习
1. 设置cmake版本：cmake_minimum_required(VERSION 3.5)
2. 设置工程名字：project (hello_world)
3. 生成依赖的lib：add_library(mylib STATIC lib/lib.cpp)
4. lib依赖的头文件： target_include_directories(mylib PUBLIC lib.h)
5. 生成二进制：add_executable(hello_world main.cpp)
6. 链接依赖的lib： target_link_libraries(hello_world PRIVATE mylib)
7. 

## 设置预编译值
* 推荐
target_compile_definitions(cmake_examples_compile_flags PRIVATE EX3)
* 不推荐
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DEX2" CACHE STRING "Set C++ Compiler Flags" FORCE)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DEX2" CACHE STRING "Set C Compiler Flags" FORCE)
## Tip
日志
message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}.")


## 清理缓存
ctrl+p
\>cmake: delete cache and reconfigure


 make VERBOSE=1

 增加调试信息
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")