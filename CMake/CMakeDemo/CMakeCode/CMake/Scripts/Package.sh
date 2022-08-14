#!/bin/bash

# 项目根目录：基于脚本的位置，上调两级。
WORKSPACE=$(cd `dirname $0`; cd ../../; pwd)

# 编译脚本
BUILD_SCRIPT=${WORKSPACE}/CMake/Scripts/Build.sh

chmod +x ${WORKSPACE}/Tools/BashUtils.sh
source ${WORKSPACE}/Tools/BashUtils.sh

run_build() {
    "${BUILD_SCRIPT}" $*
}

main() {
    info "package enter"
    
    info "shell scripts formating ..."
    format_shell_script ${WORKSPACE}

    info "build running ..."
    run_build $*

    info "package leave"
}

main $*