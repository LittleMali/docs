# 打印服务
CUPS（Common Unix Printing System）是一个开源的打印服务，最初由Apple开发，现在已成为许多Linux和Unix系统中常用的打印服务。CUPS提供了一个统一的接口，用于管理打印机、打印队列和打印作业等。

以下是CUPS的一些主要特点和功能：
1. 跨平台支持： CUPS可以在多个操作系统上运行，包括Linux、Unix、macOS等。
2. 网络打印支持： CUPS支持通过网络连接的打印机，可以通过网络共享打印机资源。
3. 驱动程序支持： CUPS提供了广泛的打印机驱动程序，支持各种打印机品牌和型号。
4. 打印队列管理： CUPS允许您创建和管理打印队列，可以设置打印优先级、暂停和恢复打印作业等。
5. 打印作业控制： CUPS允许您查看和控制打印作业，包括取消、暂停和重新启动打印作业。
6. 打印过滤器： CUPS使用过滤器来转换打印作业的格式，以适应不同的打印机和文件类型。
7. Web界面管理： CUPS提供了一个基于Web的管理界面，可以通过浏览器进行打印机和打印队列的配置和管理。
8. 安全性： CUPS支持基于用户和组的访问控制，可以限制对打印资源的访问权限。

## CUPS
CUPS的设计目标是提供一个简单、灵活和可靠的打印服务，使用户能够方便地管理和控制打印任务。它已成为许多Linux和Unix系统中默认的打印服务，并得到了广泛的应用和支持。
在Linux中，CUPS是一个常用的打印服务，它可以管理打印机、打印队列和打印作业等。当您使用CUPS打印文件时，CUPS会将打印作业转换为打印机可以理解的格式，并将其发送到打印机进行打印。

如果您选择将打印文件保存为PDF文件，CUPS会将打印作业转换为PDF格式，并将其保存到指定的位置。在默认情况下，CUPS会将生成的PDF文件保存在`/var/spool/cups-pdf/`目录下。

在该目录下，每个PDF文件都会被保存在一个以打印作业ID命名的子目录中。例如，如果打印作业ID为1234，则生成的PDF文件将保存在`/var/spool/cups-pdf/00000001/1234.pdf`中。

### cups配置
* 缓存的pdf文件路径：`/var/spool/cups-pdf/`  
  此文件打印完成以后就会删除。
* cups配置文件：`/etc/cups/cups-pdf.conf`

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