# CMake 编译打包

编译流程主要是依据参数来生成不同的工程文件，比如Debug/Release，是否编译公共库，是否编译第三方库，编译win/linux平台等等。
打包流程主要是将编译生成的文件制作成安装包。

## 编译流程
```CMake
# 目录结构
CMakeDemo
|_ build
|_ CMake
    |_ Modules
    |_ Scripts
|_ Common
|_ Src
|_ CMakeLists.txt
|_ .gitignore
```

* CMake/Scripts：编译打包脚本放在这个里面。
