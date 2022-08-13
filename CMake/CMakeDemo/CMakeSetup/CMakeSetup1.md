# 项目
本文记录从零开始如何利用CMake搭建一个项目架构。

## 入口
我们的项目位于CMakeDemo下。
* src：存放源码的dir。
* build：编译的缓存目录，一个空dir。
* CMakeLists.txt：项目配置的入口。
```CMake
# 目录结构
CMakeDemo
|_ build
|_ src
|_ CMakeLists.txt
|_ .gitignore
```