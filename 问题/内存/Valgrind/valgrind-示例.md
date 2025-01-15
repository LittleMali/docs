# 内存泄漏

Linux检测内存泄漏的工具有几个。
工具|应用环境|编程语言|使用方法|结果输出|设计思路|优缺点
----------------|-----------|----------------|--------------|-----------|------------|----------------
**mtrace**|Linux glibc|C|包含mcheck.h，程序开始时调用mtrace()|定义环境变量MALLOC_TRACE为输出文件名|为malloc,realloc,free函数添加钩子函数,记录每一对malloc-free的执行|只能检查使用malloc/realloc/free造成的的内存泄露
**memwatch**|Linux|C|包含memwatch.h，编译时加上-DMEMWATCH -DMW_STDIO及memwatch.c|输出文件名称为memwatch.log，在程序执行期间，错误提示都会显示在stdout上|将malloc/realloc/calloc/strdup/free等重定义为mwMalloc等，内部维护一个操作链表|能检测双重释放、错误释放、内存泄漏、溢出、下溢等
**valgrind**|Linux|C/C++| 编译时加上-g选项，如 gcc -g filename.c  -o filename,使用如下命令检测内存使用情况|--log-file=xxx.log指定输出文件|根据软件的内存操作维护一个有效地址空间表和无效地址空间表（进程的地址空间）|能够检测未初始化的内存，已经释放了的内存，内存越界，非法访问，多次释放，不匹配的是否，内存泄漏等。

我只用了valgrind，下面记录一下valgrind的使用方法。从上面几个工具对比来看，valgrind是比较合适的，它是对进程的运行时检测，无需修改源码，使用起来比较方便。

## valgrind

### valgrind概述
Valgrind是用于构建程序动态分析工具的重量级插桩框架。目前Valgrind内置的工具，可用于检测内存管理和线程竞争等等过程中产生的问题，更快速、更准确和更详细地分析所需的程序。  
Valgrind可以理解一个壳进程，它加载不同的工具可以做不同的检测项，Valgrind包括如下一些工具。
1. Memcheck：这是valgrind应用最广泛的工具，一个重量级的内存检查器，能够发现开发中绝大多数内存错误使用情况，比如：使用未初始化的内存，使用已经释放了的内存，内存访问越界等。这也是本文将重点介绍的部分。
2. Callgrind：它主要用来检查程序中函数调用过程中出现的问题。
3. Cachegrind：它主要用来检查程序中缓存使用出现的问题。
4. Helgrind：它主要用来检查多线程程序中出现的竞争问题。
5. Massif：它主要用来检查程序中堆栈使用中出现的问题。
6. Extension：可以利用core提供的功能，自己编写特定的内存调试工具。

### 获取
官网链接： https://valgrind.org/

### 安装
先尝试方法1，利用系统软件源直接安装。  
本人一开始使用方法2的源码安装，没成功，因为是新系统，而且，还没有安装依赖autoconf和automake。
```
# 方法1
sudo apt-get install -y valgrind

# 方法2：源码安装
## 安装依赖
sudo apt-get install -y autoconf
sudo apt-get install -y automake
## 去官网下载一个最新版本的源码
tar jvxf valgrind-3.18.1.tar.bz2
cd valgrind-3.18.1
./configue --prefix=`pwd`/build
make -j4
make install

# 确认
valgrind -v
```
### 使用
命令行运行工具拉起待测试的进程，如下所示。  
`valgrind [valgrind-options] your-prog [your-prog-options]`


一个demo例子`sudo valgrind --tool=memcheck --log-file=mem_%p --leak-check=full /opt/mytest/test`。  
如果想看到完整的函数调用栈，需要把pdb放到进程所在目录，valgrind会自动加载解析。  
valgrind支持的参数很多，有需要可以网上去找。这里只说我用到的几个。
* --tool=memcheck: valgrind支持多种工具，默认是memcheck，用于检测内存相关的问题。
* --log-file: 将结果输出到文件，%p会自动替换为进程pid。还有其他扩展，可以官方文档。
* --leak-check=full: 内存泄漏时，打印更详细的信息，否则，只能看到有内存泄漏了，但是不知道哪里泄漏了。

**注意：**如果使用valgrind时提示Permission Denied，要去确认一下进程是否有读写权限，可以通过`chmod 777 /test_proc`加权限以后再试。

接下来，我们看几个网上的例子。
#### 未初始化内存的使用
```c
int main(void)
{
    char *p; // 未初始化的变量直接使用
    char c = *p; 
    printf("\n [%c]\n",c); 
    return 0;
}
```
关键的几行日志如下，重点看： uninitialised 。说明，从网上拷贝的例子，对代码有做格式化和精简，所以，valgrind日志中的代码行数和文档对不上，这里只为说明有什么问题，看什么日志。
```bash
# gcc -g -c -o test.o test.c
# gcc -o test test.o
# valgrind --tool=memcheck ./test
==5918== Use of uninitialised value of size 8
==5918==    at 0x400539: main (test.c:8)
==5918== 
==5918== Invalid read of size 1
==5918==    at 0x400539: main (test.c:8)
==5918==  Address 0x0 is not stack'd, malloc'd or (recently) free'd
```

#### 内存被释放后进行读写
```c
int main(void)
{
    char *p = malloc(1);
    *p = 'a';
    char c = *p;
    printf("\n [%c]\n",c);
    free(p);
    c = *p; // 已经释放了再次使用
    return 0;
}
```
关键的几行日志如下，重点看： Invalid read 。
```bash
# gcc -g -c -o test.o test.c
# gcc -o test test.o
# valgrind --tool=memcheck ./test
==6067== Invalid read of size 1
==6067==    at 0x400609: main (test.c:14)
==6067==  Address 0x51f6040 is 0 bytes inside a block of size 1 free'd
==6067==    at 0x4C28F7D: free (vg_replace_malloc.c:530)
==6067==    by 0x400604: main (test.c:13)
==6067==  Block was alloc'd at
==6067==    at 0x4C27E83: malloc (vg_replace_malloc.c:299)
==6067==    by 0x4005CE: main (test.c:6)
```

#### 内存访问越界
```c
int main(void)
{
    char *p = malloc(1);
    *p = 'a'; 
    char c = *(p+1); // 访问越界
    printf("\n [%c]\n",c); 
    free(p);
    return 0;
}
```
关键的几行日志如下，重点看： Invalid read 。因为我们的读内存越界。
```bash
# gcc -g -c -o test.o test.c
# gcc -o test test.o
# valgrind --tool=memcheck ./test
==6302== Invalid read of size 1
==6302==    at 0x4005DE: main (test.c:9)
==6302==  Address 0x51f6041 is 0 bytes after a block of size 1 alloc'd
==6302==    at 0x4C27E83: malloc (vg_replace_malloc.c:299)
==6302==    by 0x4005CE: main (test.c:6)
```

#### 内存泄漏
```c
int main(void)
{
    char *p = malloc(1);
    *p = 'a'; 
    char c = *p; 
    printf("\n [%c]\n",c); 
    return 0;
}
```
关键的几行日志如下，重点看： LEAK SUMMARY。
```bash
# gcc -g -c -o test.o test.c
# gcc -o test test.o
# valgrind --tool=memcheck ./test

==6376== HEAP SUMMARY:
==6376==     in use at exit: 1 bytes in 1 blocks
==6376==   total heap usage: 1 allocs, 0 frees, 1 bytes allocated
==6376== 
==6376== LEAK SUMMARY:
==6376==    definitely lost: 1 bytes in 1 blocks
==6376==    indirectly lost: 0 bytes in 0 blocks
==6376==      possibly lost: 0 bytes in 0 blocks
==6376==    still reachable: 0 bytes in 0 blocks
==6376==         suppressed: 0 bytes in 0 blocks
==6376== Rerun with --leak-check=full to see details of leaked memory
```
LEAK SUMMARY是内存泄漏的汇总信息，每次调用valgrind时都会输出这一项结果，在前面的未初始化变量，访问越界等等也有这么一项，只是对于前面几个例子，这一项没有查看的必要。  
* definitely lost: 明确丢失的内存。程序中存在内存泄露，应尽快修复。当程序结束时如果一块动态分配的内存没有被释放并且通过程序内的指针变量均无法访问这块内存则会报这个错误。
* indirectly lost: 间接丢失。当使用了含有指针成员的类或结构体时可能会报这个错误。这类错误无需直接修复，它们总是与definitely lost一起出现，只要修复definitely lost即可
* possibly lost: 可能丢失。大多数情况下应视为与definitely lost一样需要尽快修复，除非你的程序让一个指针指向一块动态分配的内存（但不是这块内存的起始地址），然后通过运算得到这块内存的起始地址，再释放它。当程序结束时如果一块动态分配的内存没有被释放并且通过程序内的指针变量均无法访问这块内存的起始地址，但可以访问其中的某一部分数据，则会报这个错误。
* stil reachable: 可以访问，未丢失但也未释放。如果程序是正常结束的，那么它可能不会造成程序崩溃，但长时间运行有可能耗尽系统资源。

从valgrind的日志提示中也可以看到，要打印详细信息需要加上参数`--leak-check=full`。

#### 不匹配的内存释放
```c
int main(void)
{
    char *p = (char *)malloc(1);
	*p = 'a';
	char c = *p;
	printf("\n [%c]\n", c);
	delete p;
	return 0x0;
}
```
关键的几行日志如下，重点看： Mismatched。
```bash
# gcc -g -c -o test.o test.c
# gcc -o test test.o
# valgrind --tool=memcheck ./test

==15237== Mismatched free() / delete / delete []
==15237==    at 0x4C2942D: operator delete(void*) (vg_replace_malloc.c:576)
==15237==    by 0x4007EB: main (test.cpp:14)
==15237==  Address 0x5a15040 is 0 bytes inside a block of size 1 alloc'd
==15237==    at 0x4C27E83: malloc (vg_replace_malloc.c:299)
==15237==    by 0x4007B5: main (test.cpp:7)
```

#### 内存多次释放
```c
int main(void)
{
    char *p = (char *)malloc(1);
	*p = 'a'; 
	char c = *p;
	printf("\n [%c]\n",c);
	free(p);
	free(p);
	return 0;
}
```
关键的几行日志如下，重点看： Invalid free() 。
```bash
# gcc -g -c -o test.o test.c
# gcc -o test test.o
# valgrind --tool=memcheck ./test

==15354== Invalid free() / delete / delete[] / realloc()
==15354==    at 0x4C28F7D: free (vg_replace_malloc.c:530)
==15354==    by 0x400617: main (test.c:12)
==15354==  Address 0x51f6040 is 0 bytes inside a block of size 1 free'd
==15354==    at 0x4C28F7D: free (vg_replace_malloc.c:530)
==15354==    by 0x40060B: main (test.c:11)
==15354==  Block was alloc'd at
==15354==    at 0x4C27E83: malloc (vg_replace_malloc.c:299)
==15354==    by 0x4005D5: main (test.c:6)
```