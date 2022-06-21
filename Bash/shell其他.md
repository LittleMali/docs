

patchelf --set-rpath "\$ORIGIN" libtxcore.so


chrpath -r "\$ORIGIN" QQPCMgrNetCore



如果你觉得命令行操作还是不太直观，可以使用GUI工具 gconf-editor

(process:136055): dconf-WARNING **: 13:19:55.044: failed to commit changes to dconf: 无法在没有 X11 $DISPLAY 的情况下自动启动 D-Bus


#!/bin/bash

exists() {
    command -v "${1}">/dev/null 2>&1
}

main() {
    local run_target=$1
    local cur_user=`logname`
    if [ $? -ne 0 ]; then
        cur_user="$(who | awk 'NR==1{print $1}')"
    fi
    if [ `whoami` = "root" ]; then
        su $cur_user -c "$run_target"
    else
        $run_target
    fi
    
}

main $*
exit 0