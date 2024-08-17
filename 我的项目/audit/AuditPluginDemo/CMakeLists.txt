# 工程名
set(ProjectName "audisp-example")

project(${ProjectName})

# 包含当前源代码路径和build路径，防止比如config类似动态.h生成的引用
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# 引用库
find_package(libaudit QUIET)

# 源代码
file(GLOB_RECURSE AudispExample CONFIGURE_DEPENDS *.h *.cpp *.c)

# 排除列表
list(FILTER AudispExample EXCLUDE REGEX test/.*)

# 【option】仅仅供微软的vs工程分组使用，可有可无
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${AudispExample})

# 生成exe
add_executable(${ProjectName} 
    ${AudispExample}
    )


target_include_directories(${ProjectName} PRIVATE 
    ${LIBAUDIT_INCLUDE_DIRS}
    )

target_link_libraries(${ProjectName} PRIVATE 
    ${LIBAUDIT_LIBRARIES}
    )
    
