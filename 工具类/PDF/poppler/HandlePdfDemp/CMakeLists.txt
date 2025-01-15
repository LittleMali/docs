# 工程名，后续生成的exe也以此命名，如果需要变更，则需要另行修改
set(ProjectName "printer")

project(${ProjectName})

# 包含当前源代码路径和build路径，防止比如config类似动态.h生成的引用
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# 引用库
find_package(libcairo REQUIRED)
find_package(libpango REQUIRED)
find_package(popplercpp REQUIRED)
find_package(libglib REQUIRED)

# 源代码
file(GLOB_RECURSE Sources CONFIGURE_DEPENDS *.h *.cpp *.c *.cc)

# 【option】仅仅供微软的vs工程分组使用，可有可无
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${Sources})

# 生成exe
add_executable(${ProjectName}
    ${Sources}
)

target_include_directories(${ProjectName} PRIVATE
    ${LIBPANGO_INCLUDE_DIRS}
    ${LIBCAIRO_INCLUDE_DIRS}
    ${LIBPOPPLERCPP_INCLUDE_DIRS}
    ${LIBGLIB_INCLUDE_DIRS}
)

target_link_libraries(${ProjectName} PRIVATE
    ${LIBPANGOCAIRO_LIBRARIES}
    ${LIBPANGOFT2_LIBRARIES}
    ${LIBPANGO_LIBRARIES}
    ${LIBCAIRO_LIBRARIES}
    ${LIBPOPPLERCPP_LIBRARIES}
    ${LIBGLIB_LIBRARIES}
)
