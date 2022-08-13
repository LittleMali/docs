cmake_minimum_required(VERSION 3.10)

project(LittleCMakeDemo)

# set default options
option(BUILD_SRC "build src code" ON)
option(BUILD_COMMON_LIB "build common libs" ON)
option(BUILD_3RD "build 3rd dependencies" OFF)

# CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/Modules")
include(Platform)

# print messages
message(STATUS "PLATFORM_PROCESSOR: ${PLATFORM_PROCESSOR}")
message(STATUS "PLATFORM_PROCESSOR_NAME: ${PLATFORM_PROCESSOR_NAME}")
message(STATUS "PLATFORM_PROCESSOR_BIT: ${PLATFORM_PROCESSOR_BIT}")
message(STATUS "PLATFORM_SYSTEM: ${PLATFORM_SYSTEM}")
message(STATUS "PLATFORM_SYSTEM_NAME: ${PLATFORM_SYSTEM_NAME}")

