# windbg死锁分析

## 背景
某次进程启动以后，ui卡住。抓dmp分析。

## 分析
1. 查看函数栈
```
0:000> kvn
 # Child-SP          RetAddr               : Args to Child                                                           : Call Site
00 00000000`0008ec08 00000000`745c2bf1     : 00000000`779901e4 00000000`74630023 00000000`00000246 00000000`0018fff0 : wow64cpu!CpupSyscallStub+0x9
01 00000000`0008ec10 00000000`7463d286     : 00000000`00000000 00000000`745c1920 00000000`777c3128 00000000`777fc4f1 : wow64cpu!Thunk0ArgReloadState+0x23
02 00000000`0008ecd0 00000000`7463c69e     : 00000000`00000000 00000000`00000000 00000000`74634b10 00000000`7ffe0030 : wow64!RunCpuSimulation+0xa
03 00000000`0008ed20 00000000`777f43c3     : 00000000`001d2ed0 00000000`00000000 00000000`778f2e70 00000000`777c7550 : wow64!Wow64LdrpInitialize+0x42a
04 00000000`0008f270 00000000`77859780     : 00000000`00000000 00000000`77866c7d 00000000`0008f820 00000000`00000000 : ntdll!LdrpInitializeProcess+0x17e3
05 00000000`0008f760 00000000`7780371e     : 00000000`0008f820 00000000`00000000 00000000`7efdf000 00000000`00000000 : ntdll! ?? ::FNODOBFM::`string'+0x22790
06 00000000`0008f7d0 00000000`00000000     : 00000000`00000000 00000000`00000000 00000000`00000000 00000000`00000000 : ntdll!LdrInitializeThunk+0xe
```

2. 看到的是64b地址，我们先切换到32b模式
```
0:000> .load wow64exts
0:000> !sw
Switched to Guest (WoW) mode
```

3. 看一下线程。  
   总共只有5个线程，并不多。  
   能看到有LoadLibrary，也能看到DllMain里面调用了shell接口。例如下面这个1号线程。那么，可能是load dll的时候死锁了。
```
0:000:x86> ~*kvn
   1  Id: aa0.548 Suspend: -1 Teb: 7efd8000 Unfrozen
 # ChildEBP RetAddr      Args to Child              
00 020eefac 779cebd6     0000017c 00000000 00000000 ntdll_77980000!NtWaitForSingleObject+0x15 (FPO: [3,0,0])
01 020ef010 779ceaba     00000000 00000000 020ef1f4 ntdll_77980000!RtlpWaitOnCriticalSection+0x13e (FPO: [Non-Fpo])
02 020ef038 763cc44d     00299ebc 00299e98 020ef178 ntdll_77980000!RtlEnterCriticalSection+0x150 (FPO: [Non-Fpo])
03 020ef048 763cc6e5     00299ebc 8d596491 766eaf98 shell32!kfapi::CCriticalSectionLock::CCriticalSectionLock+0x14 (FPO: [Non-Fpo])

07 020ef3b4 763ccbce     020ef428 20008000 00000000 shell32!kfapi::CKFFacade::GetFolderPath+0xd5 (FPO: [Non-Fpo])

14 020ef9d8 779bfe97     64f65147 64e40000 00000001 ntdll_77980000!LdrpCallInitRoutine+0x14
15 020efacc 779bea4e     00000000 75aa9f5c 020efc70 ntdll_77980000!LdrpRunInitializeRoutines+0x26f (FPO: [Non-Fpo])
16 020efc40 779fd3df     020efcb0 020efc70 00000000 ntdll_77980000!LdrpLoadDll+0x472 (FPO: [Non-Fpo])
17 020efc7c 77432e6a     00000000 020efcd0 020efcb0 ntdll_77980000!LdrLoadDll+0xc7 (FPO: [Non-Fpo])
18 020efcc4 75fe483c     00000000 00000000 00000000 KERNELBASE!LoadLibraryExW+0x233 (FPO: [Non-Fpo])
19 020efcd8 00404965     005ff7a0 8c596acf 020efd48 kernel32!LoadLibraryW+0x11 (FPO: [Non-Fpo])

1c 020eff94 779b9802     00000000 75aa9cc8 00000000 kernel32!BaseThreadInitThunk+0xe (FPO: [Non-Fpo])
1d 020effd4 779b97d5     00401154 00000000 00000000 ntdll_77980000!__RtlUserThreadStart+0x70 (FPO: [Non-Fpo])
1e 020effec 00000000     00401154 00000000 00000000 ntdll_77980000!_RtlUserThreadStart+0x1b (FPO: [Non-Fpo])
```

4. 确认下我们的猜测。  
   刚才我们在函数栈里面看到了临界区，我们找一下是否有死锁的临界区。  
   使用 !cs -l 可以仅仅显示锁住的临界区。  
```
0:000:x86> !cs -l
-----------------------------------------
DebugInfo          = 0x77a84380
Critical section   = 0x77a820c0 (ntdll_77980000!LdrpLoaderLock+0x0)
LOCKED
LockCount          = 0x2
WaiterWoken        = No
OwningThread       = 0x00000548
RecursionCount     = 0x1
LockSemaphore      = 0x104
SpinCount          = 0x00000000
-----------------------------------------
DebugInfo          = 0x00287028
Critical section   = 0x00299ebc (+0x299EBC)
LOCKED
LockCount          = 0x1
WaiterWoken        = No
OwningThread       = 0x000005f4
RecursionCount     = 0x1
LockSemaphore      = 0x17C
SpinCount          = 0x00000000
```

5. 我们看到有两个临界区处于锁住的状态。是不是他们两个死锁了呢，我们看看它们是哪个线程的，线程栈怎么样。
6. 这里有多个方法可以看。Critical section表示临界区地址，LOCKED表示处于加锁状态。
   1. OwningThread表示线程地址，我们可以通过 ~~[线程地址] 的方式来查看线程信息。
   2. !cs -o 临界区地址 ， 也可以直接查看临界区所在的线程栈。
7. 查看第一个临界区信息。
  这是1号线程的函数栈，DllMain里面调用了shell32 api，进入了临界区并等待，1号线程试图获取地址为[00299ebc]的锁。
```
0:000:x86> !cs -o 0x77a820c0
-----------------------------------------
Critical section   = 0x77a820c0 (ntdll_77980000!LdrpLoaderLock+0x0)
DebugInfo          = 0x77a84380
LOCKED
LockCount          = 0x2
WaiterWoken        = No
OwningThread       = 0x00000548
RecursionCount     = 0x1
LockSemaphore      = 0x104
SpinCount          = 0x00000000
OwningThread DbgId = ~1s
OwningThread Stack =
	ChildEBP RetAddr      Args to Child              
	020eefac 779cebd6     0000017c 00000000 00000000 ntdll_77980000!NtWaitForSingleObject+0x15 (FPO: [3,0,0])
	020ef010 779ceaba     00000000 00000000 020ef1f4 ntdll_77980000!RtlpWaitOnCriticalSection+0x13e (FPO: [Non-Fpo])
	020ef038 763cc44d     00299ebc 00299e98 020ef178 ntdll_77980000!RtlEnterCriticalSection+0x150 (FPO: [Non-Fpo])
	020ef048 763cc6e5     00299ebc 8d596491 766eaf98 shell32!kfapi::CCriticalSectionLock::CCriticalSectionLock+0x14 (FPO: [Non-Fpo])

	020ef3dc 763a568d     020ef428 20008000 00000000 shell32!SHGetKnownFolderPath_Internal+0x96 (FPO: [Non-Fpo])
	020ef3f8 763a57cc     020ef428 00000000 00000000 shell32!SHGetFolderPathEx+0x30 (FPO: [Non-Fpo])

	020ef938 64f64f64     00000000 8c59e612 00000000 XXXXXXXX!_cinit+0x66 (FPO: [Non-Fpo]) (CONV: cdecl)
	020ef96c 64f650aa     64e40000 00000001 00000000 XXXXXXXX!_CRT_INIT+0x77 (FPO: [Non-Fpo]) (CONV: stdcall)
	020ef9ac 64f65165     64e40000 020ef9d8 779b9264 XXXXXXXX!__DllMainCRTStartup+0x59 (FPO: [Non-Fpo]) (CONV: cdecl)
	020ef9b8 779b9264     64e40000 00000001 00000000 XXXXXXXX!_DllMainCRTStartup+0x1e (FPO: [Non-Fpo]) (CONV: stdcall)
```
8. 查看第二个临界区信息。
   这是3号线程的函数栈，DllMain里面调用了shell32 api，进入了临界区并等待，试图获取地址为[77a820c0]的锁。
```
0:000:x86> !cs -o 0x00299ebc
-----------------------------------------
Critical section   = 0x00299ebc (+0x299EBC)
DebugInfo          = 0x00287028
LOCKED
LockCount          = 0x1
WaiterWoken        = No
OwningThread       = 0x000005f4
RecursionCount     = 0x1
LockSemaphore      = 0x17C
SpinCount          = 0x00000000
OwningThread DbgId = ~3s
OwningThread Stack =
	ChildEBP RetAddr      Args to Child              
	02a3f164 779cebd6     00000104 00000000 00000000 ntdll_77980000!NtWaitForSingleObject+0x15 (FPO: [3,0,0])
	02a3f1c8 779ceaba     00000000 00000000 0000002e ntdll_77980000!RtlpWaitOnCriticalSection+0x13e (FPO: [Non-Fpo])
	02a3f1f0 779be912     77a820c0 75079078 02a3f394 ntdll_77980000!RtlEnterCriticalSection+0x150 (FPO: [Non-Fpo])
	02a3f364 779fd3df     02a3f3d4 02a3f394 00000000 ntdll_77980000!LdrpLoadDll+0x2a2 (FPO: [Non-Fpo])
	02a3f3a0 77432e6a     00000000 02a3f3f4 02a3f3d4 ntdll_77980000!LdrLoadDll+0xc7 (FPO: [Non-Fpo])
	02a3f3e8 77432ec1     00000000 00000000 00000000 KERNELBASE!LoadLibraryExW+0x233 (FPO: [Non-Fpo])
	02a3f408 763a3a6b     766db930 00000000 00000000 KERNELBASE!LoadLibraryExA+0x26 (FPO: [Non-Fpo])
	02a3f454 763a3a5f     00000000 766ea27c 00000002 shell32!__delayLoadHelper2+0x59 (FPO: [Non-Fpo])
	02a3f4f0 763a4452     02a3f978 00020019 02a3f5c8 shell32!_tailMerge_ole32_dll+0xd

	02a3fbc0 00401bf0     00000000 00008023 00000000 shell32!SHGetFolderPathW+0x114 (FPO: [Non-Fpo])
```
9. 1号线程试图等待[00299ebc]释放锁，同时，3号线程试图等待[77a820c0]释放锁。
10. 但是，锁[00299ebc]被3号线程Owning，锁[77a820c0]被1号线程Owning。
11. 1号和3号线程都拥有一把锁，同时在等待对方的锁，这样就产生了死锁。
12. 再进一步的看。我们看一下临界区的数据结构。
```
0:000:x86> dt RTL_CRITICAL_SECTION
XXXXXX!RTL_CRITICAL_SECTION
   +0x000 DebugInfo        : Ptr32 _RTL_CRITICAL_SECTION_DEBUG
   +0x004 LockCount        : Int4B
   +0x008 RecursionCount   : Int4B
   +0x00c OwningThread     : Ptr32 Void
   +0x010 LockSemaphore    : Ptr32 Void
   +0x014 SpinCount        : Uint4B
```
  LockCount: 标识关键区的锁状态。  
  RecursionCount: 记录递归次数，用来支持同一个线程多次进入关键区。  
  OwningThread: 记录拥有关键区的线程ID。  
  LockSemaphore: 记录这个关键区对应的事件对象，当有线程需要等待这个关键区时，便等待这个事件，这个对象是按需创建的，如果为空，表示这个关键区没人用过,从没有线程在此等待过。

13. 我们看一下前面两个锁等待的LockSemaphore分别是什么。两个线程所WaitForSingleobject分别是[0000017c]和[00000104]，他们分别在等待handle被激活。这也是死锁的进一步表现。
```
-----------------------------------------
DebugInfo          = 0x77a84380
Critical section   = 0x77a820c0 (ntdll_77980000!LdrpLoaderLock+0x0)
LOCKED
LockCount          = 0x2
OwningThread       = 0x00000548
LockSemaphore      = 0x104
-----------------------------------------
DebugInfo          = 0x00287028
Critical section   = 0x00299ebc (+0x299EBC)
LOCKED
LockCount          = 0x1
OwningThread       = 0x000005f4
LockSemaphore      = 0x17C

# 1号线程
020eefac 779cebd6     0000017c 00000000 00000000 ntdll_77980000!NtWaitForSingleObject+0x15 (FPO: [3,0,0])
# 3号线程
02a3f164 779cebd6     00000104 00000000 00000000 ntdll_77980000!NtWaitForSingleObject+0x15 (FPO: [3,0,0])
```

14. 建议使用 !cs 查看临界区。win还有其他命令，但是不好看。  
[displaying-a-critical-section](https://learn.microsoft.com/en-us/windows-hardware/drivers/debugger/displaying-a-critical-section)  
  参考win的文档，下面两个方式也可以查看某个临界区的信息，但是，它们的LockCount并不对。
```
0:000:x86> dt RTL_CRITICAL_SECTION 00299ebc
XXXXXX!RTL_CRITICAL_SECTION
   +0x000 DebugInfo        : 0x00287028 _RTL_CRITICAL_SECTION_DEBUG
   +0x004 LockCount        : 0n-6
   +0x008 RecursionCount   : 0n1
   +0x00c OwningThread     : 0x000005f4 Void
   +0x010 LockSemaphore    : 0x0000017c Void
   +0x014 SpinCount        : 0

0:000:x86> !critsec 00299ebc

DebugInfo for CritSec at 00299ebc does not point back to the critical section
NOT an initialized critical section.

CritSec +299ebc at 00299ebc
WaiterWoken        Yes
LockCount          -1
RecursionCount     1524
OwningThread       17c
EntryCount         4890c2a4
ContentionCount    88000000
*** Locked
```