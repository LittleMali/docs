# 系统/平台信息定义，用于CMake脚本判断。

# 系统定义
# PLATFORM_SYSTEM Linux-2.26
# PLATFORM_SYSTEM_NAME linux
set(PLATFORM_SYSTEM ${CMAKE_SYSTEM})

if(CMAKE_SYSTEM_NAME MATCHES "^Android")
    set(PLATFORM_SYSTEM_NAME "android")
    set(PLATFORM_SYSTEM_NAME.Android 1)
elseif(CMAKE_SYSTEM_NAME MATCHES "^Linux")
    set(PLATFORM_SYSTEM_NAME "linux")
    set(PLATFORM_SYSTEM_NAME.Linux 1)
elseif(CMAKE_SYSTEM_NAME MATCHES "^Darwin")
    set(PLATFORM_SYSTEM_NAME "darwin")
    set(PLATFORM_SYSTEM_NAME.Darwin 1)
elseif(CMAKE_SYSTEM_NAME MATCHES "^iOs")
    set(PLATFORM_SYSTEM_NAME "ios")
    set(PLATFORM_SYSTEM_NAME.Ios 1)
elseif(CMAKE_SYSTEM_NAME MATCHES "^Windows")
    set(PLATFORM_SYSTEM_NAME "windows")
    set(PLATFORM_SYSTEM_NAME.Windows 1)
endif()

# 处理器架构
# PLATFORM_PROCESSOR aarch64
# PLATFORM_PROCESSOR_NAME aarch64
# PLATFORM_PROCESSOR_NAME.aarch64 1
string(TOLOWER ${CMAKE_SYSTEM_PROCESSOR} LOWERCASE_CMAKE_SYSTEM_PROCESSOR)
set(PLATFORM_PROCESSOR ${LOWERCASE_CMAKE_SYSTEM_PROCESSOR})

if(PLATFORM_PROCESSOR MATCHES "i[3-6]86|x86|x64|x86_64|ia64|amd64|e2k")
    set(PLATFORM_PROCESSOR_NAME "x86")

    if(PLATFORM_PROCESSOR MATCHES "e2k")
        set(PLATFORM_PROCESSOR_NAME.e2k 1)
    elseif(PLATFORM_PROCESSOR MATCHES "x64|x86_64|ia64|amd64")
        set(PLATFORM_PROCESSOR_NAME.x64 1)
    elseif(PLATFORM_PROCESSOR MATCHES "i[3-6]86|x86")
        set(PLATFORM_PROCESSOR_NAME.x86 1)
    endif()
elseif(PLATFORM_PROCESSOR MATCHES "^(mips|loongarch64)")
    set(PLATFORM_PROCESSOR_NAME "mips")

    if(PLATFORM_PROCESSOR MATCHES "mips32")
        set(PLATFORM_PROCESSOR_NAME.mips32 1)
    elseif(PLATFORM_PROCESSOR MATCHES "mips64")
        set(PLATFORM_PROCESSOR_NAME.mips64 1)
    elseif(PLATFORM_PROCESSOR MATCHES "loongarch64")
        set(PLATFORM_PROCESSOR_NAME.loongarch64 1)
    endif()
elseif(PLATFORM_PROCESSOR MATCHES "^arm|^arm64|^aarch64")
    set(PLATFORM_PROCESSOR_NAME "arm")
    
    if(PLATFORM_PROCESSOR MATCHES "^(arm|aarch)64")
        set(PLATFORM_PROCESSOR_NAME.aarch64 1)
    elseif(PLATFORM_PROCESSOR MATCHES "^arm")
        set(PLATFORM_PROCESSOR_NAME.arm 1)
    endif()
endif()

# 处理器位数
# PLATFORM_PROCESSOR_BIT "64b"
# PLATFORM_PROCESSOR_BIT.64b 1
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(PLATFORM_PROCESSOR_BIT "64b")
    set(PLATFORM_PROCESSOR_BIT.64b 1)
    set(PLATFORM_PROCESSOR_BIT_64 1) # 配合configure_file的一种定义方法
else()
	set(PLATFORM_PROCESSOR_BIT "32b")
    set(PLATFORM_PROCESSOR_BIT.32b 1)
    set(PLATFORM_PROCESSOR_BIT_32 1)
endif()
