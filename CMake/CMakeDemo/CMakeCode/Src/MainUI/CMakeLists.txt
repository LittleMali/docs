# 设置工程名
set(ProjectName "MainUI")

project(${ProjectName})

# 源代码
file(GLOB_RECURSE SourceCode CONFIGURE_DEPENDS *.h *.cpp *.c *.cc)

# 生成二进制
add_executable(${ProjectName}
    ${SourceCode}
)

# 依赖库
target_include_directories(${ProjectName} PRIVATE
    dl
)