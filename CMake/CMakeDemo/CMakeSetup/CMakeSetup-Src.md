# CMake 编写源码
## 源码
源码主要是我们的工程代码和第三库，工程代码又可以分为业务代码和公共组件，公共组件一旦开发完成一般较少修改，我们放在Common中，业务代码我们放在Src中。
```
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

这里我们举几个例子，然后挨个来实现。
## Src-业务代码
```
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

## 编译Src
我们看一下项目的CMake配置。
```cmake
# CMakeLists.txt

# set default options
option(BUILD_SRC "build src code" ON)

if (BUILD_SRC)
    add_subdirectory(Src)
endif()
```

## 添加MainUI工程
### Src的代码配置
Src下CMakeLists.txt的配置，我们先把MainUI添加到编译目录。
```cmake
# Src/CMakeLists.txt
add_subdirectory(MainUI)
```

### MainUI工程配置
在MainUI里，业务c/c++代码（h和cpp等）按照需求添加。  
MainUI要怎么编译并生成二进制文件呢，这里还需要一个CMake来配置。  
我们现在的MainUI就一个简单的main.cpp，因此，CMake的配置也很简单，声明一个工程名，添加源码文件，然后生成二进制即可。
```cmake
# Src/MainUI/CMakeLists.txt

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

## 添加NetworkService工程
类似MainUI，我们再添加NetworkService工程。

### Src的代码配置
除了MainUI，我们再添加NetworkService。
```cmake
# Src/CMakeLists.txt
add_subdirectory(MainUI)
add_subdirectory(NetworkService)
```

### NetworkService工程配置
与MainUI类似，我们只用修改一下工程名即可。
```cmake
# Src/NetworkService/CMakeLists.txt

# 设置工程名
set(ProjectName "NetworkService")

project(${ProjectName})

# 源代码
file(GLOB_RECURSE SourceCode CONFIGURE_DEPENDS *.h *.cpp *.c *.cc)

# 生成二进制
add_executable(${ProjectName}
    ${SourceCode}
)

# 依赖库
target_include_directories(${ProjectName} PRIVATE
    dl
)
```

## 二进制生成
到目前为止，我们的项目已经添加了两个业务工程工程代码，可以生成对应的代码并运行起来了。
```bash
# 在项目根目录下, linux环境
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./Src/MainUI/MainUI
i am main ui.
$ ./Src/NetworkService/NetworkService
i am network service.
```

## 下一步
我们已经生成了二进制文件，接下来我们既可以继续添加动态库，第三方库等，也可以先跑通全流程。  
这里，我们先跑通全流程，接下来，我们编写一个打包脚本，通过脚本可以编译出工程二进制文件，并且，能制作成安装包。[编译打包](./CMakeSetup-Build.md)。
