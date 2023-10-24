# CUPS
cups是linux系统下的打印服务，各大主流linux系统都会内置cups服务，苹果的mac系统也是支持的。

cups提供了一套打印控制接口，可以是命令行操作，也可以是C API操作。

## CUPS操作
### CUPS API接口

* 安装cups开发环境
```
sudo apt-get install libcups2-dev
```
* CMake配置cups
```cmake
# 仅首字母大写....坑到了
find_package(Cups REQUIRED)

# 依赖库
target_include_directories(${ProjectName} PRIVATE
    ${CUPS_INCLUDE_DIRS}
)

target_link_libraries(${ProjectName} PRIVATE
    ${CUPS_LIBRARIES}
)
```
* 代码中包含头文件即可。  
`#include <cups/cups.h>`

## CUPS缓存文件
CUPS在打印时会生成一个缓存的PDF文件，通常位于/var/spool/cups目录下，文件名类似于"cXXXXXXXX"，其中"XXXXXXXX"是一个随机生成的数字。
例如下面的例子，触发文件打印时，会生成cXXXX和dXXXX的文件，dXXXX是一个pdf格式的文件，就是要打印的内容。

```
root@xxxx:/var/spool/cups# ll /var/spool/cups
总用量 484
drwx--x--- 3 root lp     4096 5月  12 11:30 ./
drwxr-xr-x 8 root root   4096 6月  22  2022 ../
-rw------- 1 root lp      965 5月  11 20:22 c00001
-rw------- 1 root lp      965 5月  11 20:22 c00002
-rw------- 1 root lp      841 5月  11 20:45 c00003
-rw------- 1 root lp     1818 5月  12 11:08 c00004
-rw------- 1 root lp     1076 5月  12 11:30 c00005
-rw-r----- 1 root lp      234 5月  11 20:22 d00001-001
-rw-r----- 1 root lp      234 5月  11 20:22 d00002-001
-rw-r----- 1 root lp   273323 5月  12 11:08 d00004-001
-rw-r----- 1 root lp   177545 5月  12 11:29 d00005-001
drwxrwx--T 3 root lp     4096 5月  23 10:09 tmp/
```