# hook方法概述

每个进程都有自己的私有地址空间，hook技术是指将某段代码注入到另一个进程的地址空间中，这样就可以在另一个进程中干活了。

## hook窗口过程
hook窗口过程并没有实现跨进程访问，它的意义是在于子类化一个窗口。
```
SetWindowLongPtr(hWnd, GWLP_WNDPROC, MySucClassProc)
```
SetWindowLongPtr只能hook本进程的窗口过程，拿我们自定义的MySucClassProc来替换掉系统默认窗口过程，从而处理我们感兴趣的消息。系统在派发窗口消息时，会依据hWnd找到对应进程的窗口过程地址，若这个地址被hook了，那么就调用到我们的MySucClassProc中去了。

假设我们在另一个进程B中去尝试hook进程A的窗口，在进程B中可以用FindWindow来找到进程A的窗口句柄，那么在hook时，系统会判断MySucClassProc是否在同一个进程（进程A）中，若不在同一个进程中则会直接忽略。这是因为系统（User32.dll）无法在进程A中去访问进程B的MySucClassProc，这是两个不同进程的地址空间啊。

微软并没有实现这个功能，通过这个API是不用多想了。显然，要解决hook另一个进程的窗口过程，我们要让自己的代码进入另一个进程，在另一个进程的地址空间中调用SetWindowLongPtr，以此来解决进程地址空间边界问题。

## 注册表注入DLL
```
HKLM\Software\Microsoft\Windows NT\CurrentVersion\Windows\
    AppInit_DLLs        dll的路径，不能含有空格
    LoadAppInit_DLLs    1         // 新建这一项
```
其原理是当User32.dll被某个进程载入时，会收到DLL_PROCESS_ATTACH消息，然后会读取这个注册表，把里面的dll都加载进来，这就实现了hook。

这个方案的缺点很明显：
1. AppInit_DLLs可以包含多个dll，以空格或者逗号分隔，但是只有第一个dll可以包含路径，也就是说，这些dll最好都放在系统路径下。
2. 注册表注入依赖User32.dll，在win中所有的GUI程序都会加载User32，但是CUI程序就一定了，有明显的局限性。
3. Use32.dll被加载的时机位于进程初始化早期，很多系统dll还没load进来，所以，调用Kernel32的函数应该没问题，但是调用其他dll要慎重，可能蓝屏。
4. 无法指定注入的进程，会注入所有的GUI程序中。

## win挂钩注入DLL
安装挂钩可以将一个dll注入到另一个进程中。
```
SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, hInstDll, NULL)
UnhookWindowsHookEx()
```
假设我们想查看一个进程处理了哪些消息，可以安装一个消息钩子，当有消息过来时，系统会判断是否有挂钩，若有则会优先调用钩子函数GetMsgProc来处理。

SetWindowsHookEx的前两个参数好理解，第一个是要安装哪个类型的钩子，第二个是处理函数。第三个参数hInstDll标识了一个dll，该dll包含了GetMsgProc函数。也就说，进程A在给进程B安装钩子时，进程A已经载入了dll并且获得了这个dll的基址hInstDll，这个dll实现了钩子函数GetMsgProc。那么，这一步要怎么做呢？进程A显式或者隐式加载这个dll都可以，显式的话就是调用LoadLibrary来获取模块基址，隐式的话就是直接链接dll对应的lib，这样在进程A启动时就会加载dll了，在DllMain中保存一下句柄即可。第四个参数是NULL，表示全局hook。

说完了参数，我们再来看一下挂钩注入会发生什么。假设进程A已经向进程B安装好了挂钩。
1. 假设是消息钩子，那么当进程B的一个线程准备向窗口发送消息时。
2. 系统检测该线程是否安装了WH_GETMESSAGE挂钩。
3. 系统检测GetMsgProc所在的dll是否已被映射到进程B的地址空间中。
4. 若dll尚未映射，系统会将dll加载到进程B的地址空间，并将*dll锁计数递增*。
5. 对GetMsgProc进行修正。这里要分一下场景，若是两个进程空间，则需要重定位GetMsgProc，算法是进程B中dll的基址加上偏移，偏移则等于进程A中GetMsgProc地址减去dll基址。若是同一个进程空间，那么hInstDll实际是相同的，此时直接调用就可以了，这种情况下是没有必要再加载一次dll的。
6. *系统在进程B中递增该DLL的锁计数器*。
7. 在进程B中调用GetMsgProc。
8. 当GetMsgProc返回时，系统*递减该dll在进程B中的锁计数器*。

## 远程线程注入DLL

## IAT hook

## inline hook