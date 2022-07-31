# CMake Set

```cmake
set(<variable> <value>... [PARENT_SCOPE]) #设置普通变量
set(<variable> <value>... CACHE <type> <docstring> [FORCE]) #设置缓存条目
set(ENV{<variable>} [<value>]) #设置环境变量
```

例子
```cmake
set(normal_var a)
message("value: ${normal_var}") # a

set(normal_var a b c)
message("value: ${normal_var}") # a b c
```
