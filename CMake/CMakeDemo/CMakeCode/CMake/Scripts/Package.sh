#!/bin/bash

# 项目根目录：基于脚本的位置，上调两级。
# 目前的workspace是git仓库的根目录。
WORKSPACE=$(cd `dirname $0`; cd ../../; pwd)

# 编译脚本
BUILD_SCRIPT=${WORKSPACE}/CMake/Scripts/Build.sh

# 打包脚本
PACK_SCRIPT=${WORKSPACE}/CMake/Scripts/Pack.sh

chmod +x ${WORKSPACE}/Tools/BashUtils.sh
source ${WORKSPACE}/Tools/BashUtils.sh

run_build() {
    "${BUILD_SCRIPT}" $*
}

run_pack() {
    "${PACK_SCRIPT}"
}

main() {
    info "package enter"
    
    info "shell scripts formating ..."
    format_shell_script ${WORKSPACE}

    info "start build..."
    run_build $*

    info "start pack..."
    run_pack

    info "package leave"
}

main $*