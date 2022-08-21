#!/bin/bash

WORKSPACE=$(cd `dirname $0`; cd ../../; pwd)  # 代码根目录，等价于git仓库目录
BUILD_DIR=${WORKSPACE}/build # 代码根目录，项目根CMakeLists的位置。
BUILD_TEMP_DIR=${WORKSPACE}/build
BUILD_SYMBOL_DIR=$WORKSPACE/pdb

#CPU_COUNT=$(cat /proc/cpuinfo | grep "processor" | wc -l)
CPU_COUNT=4

source ${WORKSPACE}/Tools/BashUtils.sh

# default args
PLATFORM_ARCH="local"
PLATFORM_OS="linux"
BUILD_CONFIG=RelWithDebInfo
PKG_TYPE="deb"
PKG_VERSION="1.0.0.1"
PKG_OUTPUT=""
RE_CONFIGURE=OFF
RE_BUILD=OFF

BUILD_SRC_OPTION=OFF
BUILD_COMMON_OPTION=OFF
BUILD_THIRD_PARTY_OPTION=OFF

# -c (build config) : debug/release
# -a (arch): 架构，arm64/x86_64...
# -t (package type): 安装包类型，deb/rpm...
# -v (package version): 版本号，a.b.c.d
# -o (output dir): 输出路径
# -m (build modules): 编译哪些模块，all,src,common,thridparty...
# -s (system) : 当前系统 linux
# -r (rebuild) : 是否重新配置，编译, all / configure,build
parse_args() {
    while getopts :a:b:m:t:v:o:r:s: OPT
    do
        case "${OPT}" 
        in
            a) PLATFORM_ARCH=${OPTARG};;
            b) BUILD_CONFIG=${OPTARG};;
            m) BUILD_MODULES=${OPTARG};;
            t) PKG_TYPE=${OPTARG};;
            v) PKG_VERSION=${OPTARG};;
            o) PKG_OUTPUT=${OPTARG};;
            s) PLATFORM_OS=${OPTARG};;
            r) REBUILD=${OPTARG};;
            ?) echo "unknown" ${OPTARG}
        esac
    done
    shift $(($OPTIND - 1))

    info "parse args finished"
    info "start check args"

    # 当前系统
    if [[ ${PLATFORM_OS} != "linux" ]]; then
        error "Do not support os: ${PLATFORM_OS}"
        exit 1
    fi
    info "platform os: ${PLATFORM_OS}"

    # 目标处理器
    if [[ ${PLATFORM_ARCH} == "local" ]]; then
        PLATFORM_ARCH=$(uname -m)
    fi
    info "get local arch: ${PLATFORM_ARCH}"

    if [[ ${PLATFORM_ARCH} != "x86" && \
        ${PLATFORM_ARCH} != "x86_64" && \
        ${PLATFORM_ARCH} != "amd64" && \
        ${PLATFORM_ARCH} != "aarch64" ]]; then
        error "Do not support platform arch: ${PLATFORM_ARCH}"
        exit 1
    fi
    info "platform arch: ${PLATFORM_ARCH}"

    # 编译模块
    local module_item=($(echo ${BUILD_MODULES} | tr "," "\n"))
    for module_item in ${module_item[@]}
    do
        if [[ ${module_item} == "all" ]]; then
            BUILD_SRC_OPTION=ON
            BUILD_COMMON_OPTION=ON
            BUILD_THIRD_PARTY_OPTION=ON
        fi
        if [[ ${module_item} == "src" ]]; then
            BUILD_SRC_OPTION=ON
        fi
    done

    # 重新配置/编译
    local re_item=($(echo ${REBUILD} | tr "," "\n"))
    for re_item in ${re_item[@]}
    do
        if [[ $re_item == "all" ]]; then
            RE_CONFIGURE=ON
            RE_BUILD=ON
        fi
        if [[ $re_item == "configure" ]]; then
            RE_CONFIGURE=ON
        fi
        if [[ $re_item == "build" ]]; then
            RE_BUILD=ON
        fi
    done
}

configure() {
    info "re configure: ${RE_CONFIGURE}"
    if [[ ${RE_CONFIGURE} == "ON" ]]; then
        info "delete cmake reconfigure..."
        rm -rf $BUILD_DIR/CMakeFiles
        rm -rf $BUILD_DIR/CMakeCache.txt
    fi

    # 交叉编译工具链
    # todo
    info "print cmake configure params: "
    info "\t-DBUILD_SRC=$BUILD_SRC_OPTION"
    info "\t-DBUILD_COMMON=$BUILD_COMMON_OPTION"
    info "\t-DBUILD_THIRD_PARTY_OPTION=$BUILD_THIRD_PARTY_OPTION"
    info "\t-DBUILD_CONFIG=$BUILD_CONFIG"
    info "\t-DPKG_VERSION=$PKG_VERSION"

    info "start cmake configure..."
    
    cmake -DBUILD_SRC=$BUILD_SRC_OPTION \
        -DBUILD_COMMON=$BUILD_COMMON_OPTION \
        -DBULD_THIRD_PARTY=$BUILD_THIRD_PARTY_OPTION \
        -DBUILD_CONFIG=$BUILD_CONFIG \
        -DPKG_VERSION=$PKG_VERSION \
        -B"$BUILD_DIR"

    if [[ $? -ne 0 ]]; then
        error "run cmake configure failed"
        exit 1
    fi
}

build () {
    info "rebuild: ${RE_BUILD}"
    if [[ $RE_BUILD == "ON" ]]; then
        info "start cmake rebuild..."
        cmake --build $BUILD_DIR --config $BUILD_CONFIG --target clean -j ${CPU_COUNT}
    fi

    info "build cmdline: cmake --build $BUILD_DIR --config $BUILD_CONFIG --target all -j ${CPU_COUNT}"
    info "start build..."
    
    cmake --build $BUILD_DIR --config $BUILD_CONFIG --target all -j ${CPU_COUNT}
    if [[ $? -ne 0 ]]; then
        error "cmake build failed"
        exit 1
    fi
}

# ./CMake/Scripts/Package.sh -a x86 -s linux -v 2.2.2.2 -r configure
main() {
    parse_args $*

    if [[ ! -d $BUILD_DIR ]]; then
        info "build dir does NOT exists, make build dir: $BUILD_DIR"
        mkdir -p $BUILD_DIR
    fi

    info "make package output dir: ${PKG_OUTPUT}"
    mkdir -p ${PKG_OUTPUT}

    info "run configure"
    configure

    info "run build"
    build
}

main $*