# Linux开发环境搭建

由于Linux系统繁杂，大类型有类debian和ret hat系列，两个类别又派生出了多个系统，其系统的内核版本有高有低，内核版本又是和glibc强相关的。

## glibc、glib和libc
linux上有几个名称容易混淆，跟C语言的历史有关系。  
C标准主要由两部分组成，一部分描述C的语法，另一部分描述C标准库（描述了一些C标准函数的原型，但是不提供实现）。
C标准库函数的实现留给了各个系统平台；这个实现就是C运行时库(C Run Time Libray) ，简称CRT；其是和平台相关的，即和操作系统相关的；C运行库从某种程度上来讲是C语言的程序和不同操作系统平台之间的抽象层。
* libc：Linux下的ANSI C函数库。现在已经不再维护了。
* glibc：Linux下的GNU C的函数库。
* glib：GTK的一个基础库，跟libc/glibc没关系。

ansi c是美国国家标准局制定的c标准，基于其实现的就是libc。  
gnu c是自由软件基金会制定的标准，实现并扩充了c标准，也就是glibc（并不限定于linux内核，在别的内核上也可以运行）。glibc是linux系统上最底层的API之一，几乎所有C程序都要调用glibc的库函数，所以glibc是Linux平台C程序运行的基础。

类比一下windows系统，在windows系统上的crt运行时库就对应Linux上的glibc库。

## glibc和内核
前面提到了，glibc是c标准的扩张，它实现了c语言，也额外实现了很多别的特性，glibc是系统内核的基础，因此，linux系统内核在实现时是必须依赖glibc的。  
那么，问题来了，如果linux系统内核升级了，新内核支持了一些新特性，比如fnotify，但是，glibc并不支持fnotify，这样，两边的版本就不一致，系统大概率会崩溃。对于内核和glibc来说，两边都要做到对方的前向兼容，这里只能尽力，但是不做保证。简单的理解，我们可以认为系统内核版本和glibc版本是对应的。

在linux上编译c/c++工程会用到gcc编译器，gcc也是要依赖glibc的。因此，如果我们选择的系统内核比较老，那就意味着glibc的版本也比较老，我们就要使用低版本的gcc。那么，我们要低到哪个程度呢？

| 系统   | 大版本    | 内核版本                   | 内嵌gcc版本 | glibc最高支持版本 |
| ------ | --------- | -------------------------- | ----------- | ----------------- |
| Ubuntu | 16.04 LTS | 4.4, 4.8, 4.10, 4.13, 4.15 | gcc 5.4.0   | 2.23              |
| Ubuntu | 18.04 LTS | 4.15, 4.18, 5.0, 5.3       | gcc 7.5.0   | 2.27              |
| Ubuntu | 20.04 LTS | 5.4, 5.13                  | gcc 9.3.0   | 2.31              |
| Kylin  | KylinV10  | 5.4                        | gcc 9.3.0   | 2.31              |
| UOS    | Uos20     | 4.19                       | gcc 8.2.0   | 2.27              |
| CentOS | CentOS7   | 3.10.0                     | gcc 4.8.5   | 2.17              |
| CentOS | CentOS8   | 4.18.0                     | gcc 8.2.0   | 2.27              |
| RHEL   | RHEL7     | 3.10.0                     | gcc 4.8.5   | 2.17              |
| RHEL   | RHEL8     | 4.18.0                     | gcc 8.2.0   | 2.27              |
| RHEL   | RHEL9     | 5.14                       | gcc 11      | 2.31              |

因此，我们选择支持Ubuntu 18及以上，centos8及以上，RetHat8及 以上。因此，我们选择gcc7.5和glibc2.27作为我们支持的下限。

如何查看系统的glibc版本，下面的命令会打印很多GLIBC_2.2.5， GLIBC_2.3等，这就是当前系统支持的glibc版本范围。
```
locate libc.a
strings /usr/lib/libc.so.6 | grep GLIBC_
```
