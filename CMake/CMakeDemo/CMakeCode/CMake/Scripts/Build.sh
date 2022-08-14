#!/bin/bash

WORKSPACE=$(cd `dirname $0`; cd ../../; pwd)

source ${WORKSPACE}/Tools/BashUtils.sh

# -b : debug/release
# -a : 架构，arm64/x86_64...
# -t : 安装包类型，deb/rpm...
# -v : 版本号，a.b.c.d
# -o : 输出路径
# -m : 编译哪些模块，all,src,common,thridparty...
parse_args() {
    while getopts :a:b:m:t:v:o:r OPT
    do
        case "${OPT}" 
        in
            a) ARCH=${OPTARG};;
            b) BUILD_TYPE=${OPTARG};;
            m) BUILD_MODULES=${OPTARG};;
            t) PKG_TYPE=${OPTARG};;
            v) PKG_VERSION=${OPTARG};;
            o) PKG_OUTPUT=${OPTARG};;
            r) REBUILD=1;;
            ?) echo "unknown" ${OPTARG}
        esac
    done
    shift $(($OPTIND - 1))

}

main() {
    parse_args $*
}

main $*