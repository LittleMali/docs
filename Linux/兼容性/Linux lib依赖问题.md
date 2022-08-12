# Linux lib依赖问题

## glib版本问题
### 现象  
某个进程无法运行，提示 error while loading shared libraries: libevent-2.1.so.7: cannot open shared object file: No such file or directory.
### 分析  
ldd查看进程依赖。 
```bash
# 看依赖关系，缺少了哪些
$ ldd myFile
./myFile /lib/aarch64-linux-gnu/libm.so.6: version `GLIBC_2.29` not found (required by xxx.so)
./myFile /lib/aarch64-linux-gnu/libstdc++.so.6: version `GLIBC_2.29` not found (required by xxx.so)
...
...

# 确认glib版本
$ strings /lib/aarch64-linux-gnu/libm.so.6 | grep GLIBC
GLIBC_2.17
GLIBC_2.18
GLIBC_2.23
GLIBC_2.24
GLIBC_2.25
GLIBC_2.27
GLIBC_2.28
GLIBC_PRIVATE

# 我们可以看到，本机的libm.so.6支持的是最高版本才是2.28，而我们进程myFile依赖的xxx.so锁需要的glib版本是2.29 。
```
### 解决  
#### 方法1：升级glib版本。  
在你准备升级GLIBC库之前，你要好好思考一下，你真的要升级GLIBC么？你知道你自己在做什么么？glibc是gnu发布的libc库，即c运行库。glibc是linux系统中最底层的api，几乎其它任何运行库都会依赖于glibc。glibc除了封装linux操作系统所提供的系统服务外，它本身也提供了许多其它一些必要功能服务的实现...

总的来说，不说运行在linux上的一些应用，或者你之前部署过的产品，就是很多linux的基本命令，比如cp, rm, ll之类，都得依赖于它网上很多人有惨痛教训，甚至升级失败后系统退出后无法重新进入了。。

1. 安装glibc-2.29
```bash
wget http://ftp.gnu.org/gnu/glibc/glibc-2.29.tar.gz
tar -zxvf glibc-2.29.tar.gz
cd glibc-2.29
mkdir build
cd build
../configure --prefix=/opt/glibc-2.29
make
make install
```
2. glibc 软连接  
安装完成后, 建立软链指向glibc-2.29, 执行如下命令:
```bash
rm -rf /lib/aarch64-linux-gnu/libc.so.6   //先删除之前的软连接
ln -s /opt/glibc-2.29/lib/libc-2.29.so  /lib/x86_64-linux-gnu/libc.so.6
```
3. 回滚
删除libc.so.6之后可能导致系统命令不可用的情况, 可使用如下方法解决:
```bash
LD_PRELOAD=/opt/glibc-2.29/lib/libc-2.29.so
ln -s /opt/glibc-2.29/lib/libc-2.29.so /lib/x86_64-linux-gnu/libc.so.6
```
```bash
LD_PRELOAD=/lib/x86_64-linux-gnu/libc-2.31.so 
ln -s /lib/x86_64-linux-gnu/libc-2.31.so /lib/x86_64-linux-gnu/libc.so.6    // libc-2.31.so 此项是系统升级前的版本
```