# UAC和Session

## UAC和权限
UAC是用户账户控制，是win操作系统控制进程权限的一个方式，一个进程能否修改系统关键资源，要看进程是否具有相关的权限。

如何判定进程权限呢？看进程的Integrity，低权限的进程无法访问高权限的资源。Integrity的本质是进程token，在创建子进程时，默认是继承父进程的token，因此，子进程默认跟父进程有着相同的权限，除非我们在创建进程时调整token的权限。

## session和桌面
session是用户会话环境，维护了一个登录用户的交互环境，如果同时登录了多个用户，不同的用户处于不同的session下，session隔离了用户资源。

在vista及以后，系统服务进程运行在session 0下，用户登录以后系统会创建session 1，session 1是专门为用户准备的，session 1下有且仅有一个winStat0（窗口工作站），专门处理用户交互界面，也就是桌面。如果再有用户登录，那就是session 2，session 3。

## uac和session
在开启UAC以后，当用户试图以管理员权限运行进程时，系统会弹UAC提示。我们看其进程链，如下。
```
|_ wininit.exe (session=0, Integrity=SYstem)
    |_ services.exe (session=0, Integrity=SYstem)
        |_ svchost.exe (session=0, Integrity=SYstem)
            |_ consent.exe (session=2, Integrity=SYstem) -- 弹uac
```
我们可以看到，uac弹框是用户session下的System token权限。

对操作系统而言，用户登录账号（即使是管理员账号），只能拿到High权限，因此，用户进程的High token实际是无法操作system token弹框的。这也就是QQ远程无法点击UAC弹框的根本原因。

因此，如果想要在远程协助的时候点击uac弹框，那就需要将远程协助的进程提权。
1. 服务进程是system权限，但是服务进程是session 0运行的，session 0是没有桌面的，所以，session下没法直接绘制ui，因此，试图以服务进程的方式运行远程协助，是肯定不行的。
2. 服务进程是system权限，若服务进程想拉起GUI进程，要调用CreateProcessAsUser，这相当于降权运行GUI进程。
3. 服务进程有system token，token中有TokenSessionId来决定进程运行在哪个session下，那么，我们可以复制服务进程的token，然后修改其中的TokenSessionId，让其运行在用户session下，这样，我们就有了一个system权限的用户进程。因此，远程协助就可以操作uac了。
4. 微软建议，服务进程和用户进程之间使用ipc通信，用户进行需要一些高权限的操作时，发ipc通知服务进程去干活。
5. 服务进程是session 0的，用户进程是session 1的，比如获取TempDir/注册表时，两个进程拿到的路径是不一样的。
6. 远程协助的进程还要注意一个细节，服务进程以修改token的方式，实际以system权限拉起远程协助，如果远程协助再其他子进程，那么子进程的权限默认继承token，子进程也是system token，这样导致子进程的权限可能过高，我们可以hook进程内的CreateProcess等进程创建函数，将子进程降权到High。