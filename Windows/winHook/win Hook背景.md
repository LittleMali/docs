# win Hook背景
## 背景知识
### Private Address Space
在Windows操作系统中，每个进程都有自己私有的地址空间（private address space）。各个进程的地址空间（user partition）是相互独立的，尽管它们的地址编码范围相同（0x00010000 ～ 0x7FFEFFFF），但是它们对应虚拟地址的后备物理存储器一般都不相同（只读模式下的Image文件或内存映射文件可能相同，但限于“只读”，不会互相干扰），物理地址到虚拟地址的映射规则也不相同。

粗略点说，可以认为“两个进程中相同的虚拟地址对应不同的物理地址”。比如，进程A中0x004ABCDE地址处是一个函数的地址，而进程B中0x004ABCDE地址处可以是任何其他内容，甚至是未预订的空间。那么，在进程A中可以通过函数指针来调用0x004ABCDE的函数，而在进程B中通过指针引用0x004ABCDE则完全没有意义。如果把进程地址空间比作平行的宇宙空间，那么各个进程空间内部的地址就是各个宇宙空间的内部坐标，坐标数值相等的两个点事实上并不是同一个点。

进程地址空间的这种独立性，增强了Windows操作系统的安全性，它保障了“不会因为一个进程又缺陷覆盖了某个随机地址，导致其他进程有效内存被覆盖”。（早期的DOS操作系统各应用程序进程运行在一个进程空间，互相之间有极大的干扰）。

### 跨进程边界的需要
这种独立的进程地址空间，让一个进程访问另一个进程变成了几乎不可能的事。就仿佛是东西走向的高速公路上被绿化带隔离的两条车道的车，东向行驶的车是不可能刮蹭西向行驶的车的。但是，有些应用程序有时候需要跨越进程边界来访问另一个进程的地址空间。  
一般可以总结为以下几种情况：
1. 我们想要从另一个进程创建的窗口派生子类窗口（subclass a window created by another process）。
2. 我们需要一些手段来辅助调试。比如我们需要确定另一个进程正在使用哪些DLL。
3. 我们想要给另一个进程安装挂钩（hook other processes）。

### 跨进程边界的方法 —— DLL注入
既然我们有跨进程边界的需要，那么我们就需要某种方法来实现它。具体怎么实现，我们还是要从操作系统的内存体系结构入手。  
前面在讲进程的“private address space”的独立性的时候，我把它比作平行宇宙空间，但事实上，Windows OS的进程地址空间并不是完全独立的，它们有如下两种类型的交叉：
1. 从虚拟地址层面来看，各个进程地址空间的“kernel partition”是共用的。即虚拟地址0x80000000～ 0xFFFFFFFF对应的后备物理存储器完全相同。但是，该分区仅限于内核代码访问，因此，不影响普通应用程序的独立性。
2. 从物理地址层面来看，各个进程共用的一些DLL文件的映像文件（Image）和共用的内存映射文件，在映射到虚拟地址空间且只读时，它们的后备物理存储器是相同的，都是某个位于磁盘上的文件。

既然有这么两个交叉，那么我们就可以利用它们来实现跨进程边界。其中，第一个交叉时属于内核区域，不适合普通的应用程序，不在本文的讨论范围。第二个交叉，我们可以给它起个名字，叫“共享DLL”，本文正是利用它来实现跨进程边界。

## DLL注入的原理
### 系统dll
系统DLL在不同进程空间的导出函数地址值相等吗？

Windows OS中有三个非常重要的系统DLL：kernel32.dll、user32.dll和gdi32.dll，它们是大多数应用程序都需要载入的DLL。因此，我们进程A中可以引用到“共享DLL”中的某个导出函数epf1的地址，那么我们在进程B中也可以引用到同一条函数epf1的地址。注意，我只是说可以引用到该函数的地址，并没有说该函数在进程A和进程B的虚拟地址是相等的。

那么，这三个共享DLL的某个函数的地址在不同的进程空间的虚拟地址是否真的相等呢？  
答案是：YES！！！

在《Windows核心编程》第19章“DLL基础”中，讲DLL的PE结构时提到：在生成DLL文件的时候，链接器除了生成一个对应的.lib文件（包含全部的导出符号），还会在DLL文件中嵌入一个导出符号表（export section，导出段）。这个（按字母顺序排列的）导出段列出了导出的变量、函数和类的符号名。链接器还会保存相对虚拟地址（Relative Virtual Address，RVA），表示每个符号可以在DLL模块的何处找到。

从上段可知，DLL的各个导出符号相对DLL内部的位置是固定的，在DLL生成的时候就确定了。此外，DLL还有一个RVA，它相对的是DLL在实际进程空间中的起始地址。在具体某个进程中，访问DLL中的某个导出函数时（也可以是导出变量和类），它引用的地址为：actual address = start address + RVA

综上，只要DLL在各个进程中的“start address”相同，那么对它的导出函数的引用地址也相同。

那么，上述三个系统DLL在不同的进程地址空间的“start address”相等吗？  
上面已给出了答案，在此只是强调一下。注意以下几点：
1. 应用程序在载入DLL文件的时候，首先载入各个系统DLL，如ntdll.dll、kernel32.dll等。
2. 为了避免地址空间碎片化，DLL载入时，系统按从高位到地位的顺序分配地址区域（region）。
3. 为了避免实际载入时对多个DLL的“基地址空间重定位”，Windows在发布的时候，会先对系统DLL进行rebasing操作。执行了rebasing操作之后，各个系统DLL载入到不同进程空间时的“start address”值相等（大多数情况下，不是绝对）。
4. ASLR随boot的不同而不同，在同一台机器同一次开机运行时，不影响系统DLL的“start address”。


### 一个进程操纵另一个进程的方法
为了保障Windows各个进程的独立性，操作系统还尽可能的让Windows API函数“只允许对调用它的进程自身进行操作”。也就是说，大部分的Windows API都“不带传入其他进程句柄或进程ID的参数“。但是，windows也为调试器或是其他工具设计了一些函数，这些函数可以让一个进程对另一个进程进行操作。虽然他们是为调试器设计的，但是任何应用程序都可以调用它们。而”DLL注入“正是要用到这类Windows API，具体包括：
#### 动态加载DLL
```
HMODULE WINAPI LoadLibrary(
  _In_ LPCTSTR lpFileName
); 
```
从根本上说，DLL注入技术要求目标进程中的一个线程调用LoadLibrary函数来载入我们想要的DLL。
#### 在目标进程中开空间和释放空间
```
LPVOID WINAPI VirtualAllocEx(
  _In_     HANDLE hProcess,
  _In_opt_ LPVOID lpAddress,
  _In_     SIZE_T dwSize,
  _In_     DWORD  flAllocationType,
  _In_     DWORD  flProtect
); 

BOOL WINAPI VirtualFreeEx(
  _In_ HANDLE hProcess,
  _In_ LPVOID lpAddress,
  _In_ SIZE_T dwSize,
  _In_ DWORD  dwFreeType
); 
```

#### 读写其他进程的内存空间
```
BOOL WINAPI ReadProcessMemory(
  _In_  HANDLE  hProcess,
  _In_  LPCVOID lpBaseAddress,
  _Out_ LPVOID  lpBuffer,
  _In_  SIZE_T  nSize,
  _Out_ SIZE_T  *lpNumberOfBytesRead
); 

BOOL WINAPI WriteProcessMemory(
  _In_  HANDLE  hProcess,
  _In_  LPVOID  lpBaseAddress,
  _In_  LPCVOID lpBuffer,
  _In_  SIZE_T  nSize,
  _Out_ SIZE_T  *lpNumberOfBytesWritten
); 
```

#### 控制其他进程开新线程
```
HANDLE WINAPI CreateRemoteThread(
  _In_  HANDLE                 hProcess,
  _In_  LPSECURITY_ATTRIBUTES  lpThreadAttributes,
  _In_  SIZE_T                 dwStackSize,
  _In_  LPTHREAD_START_ROUTINE lpStartAddress,
  _In_  LPVOID                 lpParameter,
  _In_  DWORD                  dwCreationFlags,
  _Out_ LPDWORD                lpThreadId
); 
```

#### 设置其他进程的窗口挂钩
```
HHOOK WINAPI SetWindowsHookEx(
  _In_ int       idHook,
  _In_ HOOKPROC  lpfn,
  _In_ HINSTANCE hMod,
  _In_ DWORD     dwThreadId
); 
```