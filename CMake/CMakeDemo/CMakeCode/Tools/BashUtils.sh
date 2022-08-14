#!/bin/bash

info() {
    echo -e "\033[36m${1} \033[0m"
}

warn() {
    echo -e "\033[33m${1} \033[0m"
}

error() {
    echo -e "\033[31m[ERROR] ${1} \033[0m"
}

exists() {
    command -v "${1}"
}

format_shell_script() {
    local cur_dir=${1}

    find ${cur_dir} -name "*.sh" | xargs -r chmod +x

    exists dos2unix
    if [ $? -eq 0 ]; then
        find ${cur_dir} -name "*.sh" | xargs -r dos2unix -f
        find ${cur_dir} -name "configure" | xargs -r dos2unix -f
    else
        warn "NO dos2unix exists, shell scripts format UNIX does NOT run"
    fi
}