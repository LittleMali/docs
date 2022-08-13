cmake_minimum_required(VERSION 3.10)

project(LittleCMakeDemo)

#-------------------- 平台相关信息 begin --------------------
# CMake modules
# 添加平台/处理器架构预定义
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/Modules")
include(Platform)

# print messages
message(STATUS "PLATFORM_PROCESSOR: ${PLATFORM_PROCESSOR}")
message(STATUS "PLATFORM_PROCESSOR_NAME: ${PLATFORM_PROCESSOR_NAME}")
message(STATUS "PLATFORM_PROCESSOR_NAME.x64: ${PLATFORM_PROCESSOR_NAME.x64}")
message(STATUS "PLATFORM_PROCESSOR_BIT: ${PLATFORM_PROCESSOR_BIT}")
message(STATUS "PLATFORM_SYSTEM: ${PLATFORM_SYSTEM}")
message(STATUS "PLATFORM_SYSTEM_NAME: ${PLATFORM_SYSTEM_NAME}")

# 使用CMake定义的变量：CMake-->代码的转换。
# 配置文件
configure_file(CMake/PlatformConfig.h.in PlatformConfig.h)

# 宏定义另一种写法
# 等价于 #define _BIT64 1
if(PLATFORM_PROCESSOR_BIT.64b)
    add_definitions(-D_BIT64)
else()
    add_definitions(-D_BIT32)
endif()

# -------------------- 平台相关信息 end --------------------

# -------------------- 源码 Src begin --------------------

# set default options
option(BUILD_SRC "build src code" ON)
option(BUILD_COMMON_LIB "build common libs" ON)
option(BUILD_3RD "build 3rd dependencies" OFF)

if (BUILD_SRC)
    add_subdirectory(Src)
endif()

# -------------------- 源码 Src end --------------------