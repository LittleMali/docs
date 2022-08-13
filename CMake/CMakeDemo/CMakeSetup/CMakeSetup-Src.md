# CMake Src
前面Setup-0里面介绍了项目的初始目录结构，并且添加了项目总的cmake配置。  
接下来，我们在Src下开始添加工程文件。

## Src-业务代码
```CMake
# 目录结构
CMakeDemo
|_ build
|_ CMake
|_ Common
|_ Src
    |_ MainUI
        |_ main.cpp
        |_ CMakeLists.txt
    |_ CMakeLists.txt
|_ CMakeLists.txt
|_ .gitignore
```
* 在项目CMake配置中，添加Src源码的编译。
* 在Src目录下，增加CMakeLists.txt配置Src下的文件，控制编译哪些二进制。
* 在Src下，新建一个MainUI的目录，用来编写ui相关程序，在MainUI下也需要一个CMake配置。

### 编译Src
我们看一下项目的CMake配置。
```cmake
# set default options
option(BUILD_SRC "build src code" ON)

if (BUILD_SRC)
    add_subdirectory(Src)
endif()
```

### Src的代码配置
Src下CMakeLists.txt的配置，我们先把MainUI添加到编译目录。
```cmake
add_subdirectory(MainUI)
```

### MainUI工程配置
在MainUI里，业务c/c++代码（h和cpp等）按照需求添加。  
MainUI要怎么编译并生成二进制文件呢，这里还需要一个CMake来配置。  
我们现在的MainUI就一个简单的main.cpp，因此，CMake的配置也很简单，声明一个工程名，添加源码文件，然后生成二进制即可。
```cmake
# 设置工程名
set(ProjectName "MainUI")

project(${ProjectName})

# 源代码
file(GLOB_RECURSE SourceCode CONFIGURE_DEPENDS *.h *.cpp *.c *.cc)

# 生成二进制
add_executable(${ProjectName}
    ${SourceCode}
)
```