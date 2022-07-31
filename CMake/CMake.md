# CMake

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