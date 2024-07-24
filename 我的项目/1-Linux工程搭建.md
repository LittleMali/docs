# Linux项目搭建

本项目是一个跨平台的工程，预期要支持x86，arm两个架构，支持类debian和类rpm两类系统。  
由于主要在linux平台上开发，因此，我们选择c++作为开发语言，使用CMake管理工程项目，使用Qt作为UI开发引擎。  
为了在一个工程项目中生成多个架构的包，我们需要使用交叉编译，在宿主机器上完成x86和arm架构的编译和制作。同时，由于使用了CMake，所以，我们可以直接使用CPack来制作安装包。

目标：支持多架构多系统的编译构建。  
解决方案：CMake + 交叉编译 + CPack。

## 工程结构
```bash
/workspace
|- docs
|- cmake # cmake脚步
    |- toolchains # 交叉编译工具链cmake脚步
    |- modules # 依赖库查找cmake脚步
|- ci # 存放构建打包脚步，执行CMake命令
    |- scripts
|- build # 缓存文件和生成的二进制文件
|- deps # 依赖库
    |- thirdparty # 第三方库，仅编译一次
    |- core # 核心库，公共库
    |- platforms # 存放二进制产物，比如QT
        |- linux
            |- arm64
            |- x64
|- src # 源码
    |- client # ui界面
    |- services # 服务进程
        |- businessplugins # 业务插件
        |- coreplugins # 核心插件
        |- servicemain # 服务主进程
        |- uninstaller # 卸载进程
|- tools # 工具脚步
|- tests # 测试用例
```

## CMake配置
CMake是大型项目的管理工具，通过CMake可以生成makefile或者visual studio工程文件，从而方便编译器编译源码工程。

CMake主要的用处：
1. 管理工程文件，包括最终二进制文件和中间lib依赖等。
2. 交叉编译工具链配置：选择x64/arm64架构的编译器进行编译。

## 编译步骤
```bash
# 先配置整个工程项目
cmake -H"$WORKSPACE_DIR" \
    -B"$BUILD_DIR" \
    -G "Unix Makefiles" \
    -DCMAKE_TOOLCHAIN_FILE=$toolchain

# 再编译
cmake --build "$BUILD_DIR" --config $BUILD_CONFIGURE --target all -j ${NCPU} --

# 打包
cpack -G ${package_generator_item} -P ${PACKAGE_NAME_PREFIX} -DCPACK_CMAKE_GENERATOR=Ninja
```

### 交叉编译
交叉编译是指在宿主机器上编译出指定架构的二进制文件，例如，在x86的宿主机上编译出arm64的二进制文件。使用交叉编译可以在一台机器上构建出多个不同架构的包，不用专门准备多个编译环境。

在CMake中使用`DCMAKE_TOOLCHAIN_FILE`来指定工具链，在工具链中会设置要编译器和sysroot的相关信息。

我们看一个arm64的toolchain配置。
```bash
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# GNU交叉编译工具链路径，编译机上的路径。
set(GNU_COMPLIER_ROOT "/usr/local/gcc7.5.0-libc2.17-kernel3.10-x86_64-aarch64-linux-gnu")
set(GNU_COMPLIER_BIN "${GNU_COMPLIER_ROOT}/bin")
set(GNU_COMPLIER_GCC "${GNU_COMPLIER_BIN}/aarch64-linux-gnu-gcc")
set(GNU_COMPLIER_GXX "${GNU_COMPLIER_BIN}/aarch64-linux-gnu-g++")
set(GNU_COMPLIER_OBJCOPY "${GNU_COMPLIER_BIN}/aarch64-linux-gnu-objcopy")
set(GNU_COMPLIER_STRIP "${GNU_COMPLIER_BIN}/aarch64-linux-gnu-strip")

# QT交叉编译工具链路径，编译机上的路径。
set(QT5_COMPLIER_ROOT "${GNU_COMPLIER_ROOT}/sysroot/Qt5.15.7")
set(Qt5_DIR "${QT5_COMPLIER_ROOT}/lib/cmake/Qt5")

# 设置GNU交叉编译工具链
set(ENV{PATH} ${GNU_COMPLIER_BIN}:$ENV{PATH})
set(CMAKE_C_COMPILER ${GNU_COMPLIER_GCC})
set(CMAKE_CXX_COMPILER ${GNU_COMPLIER_GXX})
set(CMAKE_STRIP ${GNU_COMPLIER_STRIP})
set(CMAKE_OBJCOPY ${GNU_COMPLIER_OBJCOPY})

# 设置搜索路径规则。由于是交叉编译，lib库不能查找宿主机中的，需要自行添加路径。
set(CMAKE_SYSROOT ${GNU_COMPLIER_ROOT}/sysroot)

# 设置架构集（这里有待讨论）
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DARM_ARCH -DAARCH64 -D__aarch64__ -march=armv8-a")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DARM_ARCH -DAARCH64 -D__aarch64__ -march=armv8-a")
```
我们在编译时会依赖很多系统lib，但是，在交叉编译的时候，工程依赖的系统lib显然不能在宿主机器上寻找，因为目标文件是arm64架构，宿主机是x64架构，arm的二进制肯定依赖不了x64的lib。所以，我们要显示提供sysroot，告诉编译器在哪个位置存放着系统lib。

对于我们来说，我们的sysroot是从目标机器上拷贝出来的。我们安装了arm64的虚拟机（centos系统），从虚拟机中拷贝文件作为sysroot。主要是下面这几个目录，编译的时候会报失败，缺什么再拷贝什么。
```
/lib
/usr/lib
/usr/include
其他相关目录
```

## CPack打包
cmake编译生成了二进制文件，我们接下来需要制作安装包。Linux上主要是deb和rpm两个格式。
* .deb：Debian包格式，包含二进制文件、元数据和安装脚本。使用 dpkg 工具进行安装和管理。
* .rpm：Red Hat包管理器（RPM）包格式，包含二进制文件、元数据和安装脚本。使用 rpm 工具进行安装和管理。
```bash
set(CPACK_PACKAGE_CONTACT "Your Name <your.email@example.com>")
set(CPACK_PACKAGE_VENDOR "Your Company")
set(CPACK_PACKAGE_VERSION ${BUILD_VERSION})
set(CPACK_PACKAGE_DESCRIPTION "Your project description")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Your project description summary")

set(CPACK_INSTALL_PREFIX "/opt/apps/${PACKAGE_NAME}")
set(CPACK_CMAKE_GENERATOR "Ninja")

# debian
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_SOURCE_DIR}/ci/scripts/packaging/preinst;${CMAKE_SOURCE_DIR}/ci/scripts/packaging/postinst;${CMAKE_SOURCE_DIR}/ci/scripts/packaging/prerm;${CMAKE_SOURCE_DIR}/ci/scripts/packaging/postrm")
set(CPACK_DEBIAN_COMPRESSION_TYPE "xz")

# rpm scripts
set(CPACK_RPM_PRE_INSTALL_SCRIPT_FILE ${CMAKE_SOURCE_DIR}/ci/scripts/packaging/preinst)
set(CPACK_RPM_PRE_UNINSTALL_SCRIPT_FILE ${CMAKE_SOURCE_DIR}/ci/scripts/packaging/prerm)
set(CPACK_RPM_POST_INSTALL_SCRIPT_FILE ${CMAKE_SOURCE_DIR}/ci/scripts/packaging/postinst)
set(CPACK_RPM_POST_UNINSTALL_SCRIPT_FILE ${CMAKE_SOURCE_DIR}/ci/scripts/packaging/postrm)
set(CPACK_RPM_COMPRESSION_TYPE "xz")

# Enable the "DEB" and "RPM" generators
# 生成.deb包需要dpkg-deb工具，生成.rpm包需要rpmbuild工具。因此，在一个系统上只能生成一个包格式。
set(CPACK_GENERATOR "DEB;RPM")

# 设置架构
# set(CPACK_RPM_PACKAGE_ARCHITECTURE "i386")
set(CPACK_RPM_PACKAGE_ARCHITECTURE "x86_64")
# set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
# set(CPACK_RPM_PACKAGE_ARCHITECTURE "aarch64")

set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_${BUILD_VERSION}_${PLATFORM_PROCESSOR_NAME}-${PLATFORM_BUILD_TYPE}")

# Include the CPack module (this must be the last instruction of the file)
include(CPack)
```

## 完整的流程
整个构建的完整流程。
1. x64编译机，使用docker管理编译环境，docker可以方便的控制并发编译。
2. 一个docker实例编译一个架构。docker中cmake configure、build、cpack生成一个安装包。
3. 等待所有架构制作完成，结束。