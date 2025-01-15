# win进程退出卡死

进程退出的时候卡主了，导出一份dmp文件，同时，记住进程pid，保存对应的日志。

## 查看线程栈
```
0:000> kvn
 # Child-SP          RetAddr               : Args to Child                                                           : Call Site
00 00000000`006beb58 00000000`77271b1d     : 00000023`772f160c 00007ffc`58f30023 00000000`744c2bd9 00000000`007bee50 : wow64cpu!CpupSyscallStub+0xc
01 00000000`006beb60 00000000`77271199     : 00000000`007bf7e0 00007ffc`55f5d0c4 00000000`006bec30 00007ffc`55f5c5e7 : wow64cpu!Thunk0ArgReloadState+0x5
02 00000000`006bec10 00007ffc`55f5cfda     : 00000000`008bb000 00000000`00e70110 00000000`00000000 00000000`006bf490 : wow64cpu!BTCpuSimulate+0x9
03 00000000`006bec50 00007ffc`55f5cea0     : 00000000`00000000 00000000`00ab25c8 00000000`00000000 00000000`00000000 : wow64!RunCpuSimulation+0xa
04 00000000`006bec80 00007ffc`58fb548d     : 00000000`00000010 00000000`00000010 00007ffc`5900d110 00000000`008ba000 : wow64!Wow64LdrpInitialize+0x120
05 00000000`006bef30 00007ffc`58f5839b     : 00000000`00000001 00000000`00000000 00000000`00000000 00000000`00000001 : ntdll!LdrpInitializeProcess+0x1789
06 00000000`006bf370 00007ffc`58f58223     : 00000000`00000000 00007ffc`58ee0000 00000000`00000000 00000000`008bc000 : ntdll!LdrpInitialize+0x15f
07 00000000`006bf410 00007ffc`58f581ce     : 00000000`006bf490 00000000`00000000 00000000`00000000 00000000`00000000 : ntdll!LdrpInitialize+0x3b
08 00000000`006bf440 00000000`00000000     : 00000000`00000000 00000000`00000000 00000000`00000000 00000000`00000000 : ntdll!LdrInitializeThunk+0xe
```

## 切换到32b
```
0:000> .load wow64exts
0:000> !sw
Switched to Guest (WoW) mode
```

## 再看栈
确实是卡在进程退出上了，已经在运行反初始化了。
```
0:000:x86> kvn
 # ChildEBP RetAddr      Args to Child              
00 007bee84 75188b29     000005bc 00000000 00000000 ntdll_77280000!NtWaitForSingleObject+0xc (FPO: [3,0,0])
01 007beef8 75188a82     000005bc ffffffff 00000000 KERNELBASE!WaitForSingleObjectEx+0x99 (FPO: [SEH])
02 007bef0c 05644aa8     000005bc ffffffff 0567d42c KERNELBASE!WaitForSingleObject+0x12 (FPO: [Non-Fpo])
WARNING: Stack unwind information not available. Following frames may be wrong.
03 007bef48 05666643     0567c7e8 05666638 0566047a xxxPolicy!OnProcCreate+0x13927
04 007bef54 0566047a     00000000 00000000 05630000 xxxPolicy!std::_Init_locks::operator=+0x65bd
05 007bef68 0566056d     05630000 a094c586 00000000 xxxPolicy!std::_Init_locks::operator=+0x3f4

10 007bf168 72369842     025c9b50 72367798 723dd244 Main!Manager::ReleasePlugins+0x41 (FPO: [Non-Fpo]) (CONV: thiscall) 
11 007bf170 72367798     723dd244 723dac80 7233358a Main!Manager::Stop+0xf (FPO: [0,0,4]) (CONV: thiscall) 
12 007bf17c 7233358a     6f798dd9 723dd504 723dac80 Main!UnInit+0x2d 
```

## 看看日志
确实是在退出了，对比代码，框架dll退出时先调用了xxxPolicy.dll的Stop接口。

在Stop已经调用完成以后，xxxPolicy.dll还有一个线程打印了StopWorkThread日志，难道是线程没有退出？
![20240807202350](https://raw.githubusercontent.com/LittleMali/docs/master/mdPics/20240807202350.png)


## 再看线程
结合日志，tid=11444，对应是0x2cb4，是0号线程，也就是刚才的函数栈。

同时还有工作线程的tid=18036，对应是0x4674，是10号线程
```
0:000:x86> ~
.  0  Id: 448c.2cb4 Suspend: 0 Teb: 008bc000 Unfrozen

  10  Id: 448c.4674 Suspend: 0 Teb: 008fe000 Unfrozen
```

## 看10号线程
```
0:000:x86> ~10s
ntdll_77280000!NtWaitForSingleObject+0xc:
772f156c c20c00          ret     0Ch

0:010:x86> kvn
 # ChildEBP RetAddr      Args to Child              
00 05cdf8cc 772cabc2     00000080 00000000 00000000 ntdll_77280000!NtWaitForSingleObject+0xc (FPO: [3,0,0])
01 05cdf8f0 772c29bb     c07cae75 00000000 744c29e1 ntdll_77280000!LdrpDrainWorkQueue+0x14f (FPO: [Non-Fpo])
02 05cdf990 735e5e1f     928d872d 00000000 744c29e1 ntdll_77280000!LdrShutdownThread+0x7b (FPO: [SEH])

04 05cdfa9c 744c299f     00000000 744c29c1 00000000 ntdll_77280000!RtlExitUserThread+0x4c (FPO: [Non-Fpo])
05 05cdfaa4 744c29c0     00000000 3a091ef6 744c29e1 msvcr80!_endthreadex+0x1f (FPO: [1,0,0]) (CONV: cdecl) 
06 05cdfadc 744c2a47     744c29e1 76ce0419 025c7ce0 msvcr80!_callthreadstartex+0x20 (FPO: [Non-Fpo]) (CONV: cdecl) 
07 05cdfae4 76ce0419     025c7ce0 76ce0400 05cdfb50 msvcr80!_threadstartex+0x66 (FPO: [1,0,4]) (CONV: stdcall) 
08 05cdfaf4 772e72ed     025c7ce0 c07cacb5 00000000 kernel32!BaseThreadInitThunk+0x19 (FPO: [Non-Fpo])
```

## 0号线程在等什么
`00 007bee84 75188b29     000005bc 00000000 00000000 ntdll_77280000!NtWaitForSingleObject+0xc (FPO: [3,0,0])`
```
0:000:x86> !handle 000005bc f
Handle 000005bc
  Type         	Thread
  Attributes   	0
  GrantedAccess	0x1fffff:
         Delete,ReadControl,WriteDac,WriteOwner,Synch
         Terminate,Suspend,Alert,GetContext,SetContext,SetInfo,QueryInfo,SetToken,Impersonate,DirectImpersonate
  HandleCount  	3
  PointerCount 	131065
  Name         	<none>
  Object specific information
    Thread Id   448c.4674
    Priority    8
    Base Priority 0
```

## 10号线程在等什么
`00000080 00000000 00000000 ntdll_77280000!NtWaitForSingleObject+0xc (FPO: [3,0,0])`
```
0:000:x86> !handle 00000080 f
Handle 00000080
  Type         	Event
  Attributes   	0
  GrantedAccess	0x1f0003:
         Delete,ReadControl,WriteDac,WriteOwner,Synch
         QueryState,ModifyState
  HandleCount  	2
  PointerCount 	65100
  Name         	<none>
  Object specific information
    Event Type Auto Reset
    Event is Waiting
```

## 互等
0号线程wait的句柄是一个Thread（tid = 448c.4674），而tid = 448c.4674正好就是10号线程。

同时，10号线程在等一个Event，看代码，这个Event是退出时SetEvent触发的。

两边都在等待信号被触发，有点像死锁。我们在回去看反初始化的代码，框架dll已经调完xxxPolicy.dll的Stop接口了，为什么xxxPolicy.dll在Stop的时候没有SetEvent触发10号线程退出？

结合日志打印的退出信息，我们看到工作线程位于一个单例中，单例没有提示显示的UnInit接口，它是在析构的时候UnInit的。我们的xxxPolicy.dll已经被Release了，这个时候单例才析构，时机有点迟，这个析构里面干事情，确实不好保证时序。所以，还是要增加显示析构接口。

## 解决
增加显示析构接口，在Stop的时候明确调用UnInit，通知工作线程退出。