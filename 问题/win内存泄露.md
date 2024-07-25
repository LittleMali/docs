# win内存泄露

https://learn.microsoft.com/zh-cn/windows-hardware/drivers/debugger/using-umdh-to-find-a-user-mode-memory-leak

即使一个进程已经在运行了，你依然可以使用 umdh 来检测内存泄露。umdh 的设计就是为了在进程运行中捕获和分析堆内存的快照，从而帮助你识别内存泄漏。这种方法不需要停止或暂停进程，因此可以在不中断应用程序正常运行的情况下进行内存泄漏分析。

## 工具
* umdh.exe
* gflags.exe

## 使用方法
```bash
# 设置pdb路径
set _NT_SYMBOL_PATH=D:\symbols;D:\symbols\tmp;D:\symbols\tmp\PDB;srv*D:\symbols\microsoft*https://msdl.microsoft.com/download/symbols

# 开启gflags
# 只用进程名即可。
gflags.exe -i test.exe +ust

# 运行test.exe

# 开始第一次内存抓取
# pid是test.exe的进程
umdh.exe -p:pid -f:test.1.log

# 等待一段时间

# 第二次抓取内存
umdh.exe -p:pid -f:test.2.log

# 生成内存差异结果
umdh.exe test.1.log test.2.log > test.log

# 对比内存差异，看test.log文件
+  5a2e86 ( 2a8067e - 24dd7f8)  dc8a0 allocs	BackTrace3965308
+   29998 ( dc8a0 - b2f08)	BackTrace3965308	allocations

	ntdll!RtlpCallInterceptRoutine+26
	ntdll!RtlpAllocateHeapInternal+108C
	ntdll!RtlAllocateHeap+3E
	MSVCR80!malloc+7A (f:\dd\vctools\crt_bld\self_x86\crt\src\malloc.c, 163)

# 记得清楚gflags标记
gflags.exe -i test.exe -ust
```

上面有一个示例结果报告。

step1：  
5a2e86 是总共泄露了多少内存，发生的位置是 BackTrace3965308。  
其后的内容是函数调用栈。可以看到是malloc分配内存。

step2：  
回到test.2.log日志，去搜索BackTrace3965308，看看是谁在调用。  
test.2.log里面没有识别到pdb，只有函数地址，所以，我们最好同步的导出一份fulldmp，在fulldmp中去看这个地址：`ln 77D27AE4`。  

```
C6 bytes + 1A at 56004C0 by BackTrace3965308
	77D27AE4
	77C96FCC
	77C95F2E
	758D4D83
```

step3：  
这个调用是malloc，太多地方在调用了，没有找出来是哪个地方泄露的。
