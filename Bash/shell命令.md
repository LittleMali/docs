# shell命令

## 网络相关
查看网络状态，比如是否监听了某个端口。
```shell
-p: 协议。
-a：all，所有连接和监听的端口。
-b：连接所涉及的进程。

netstat -p tcp -a -b

netstat -ano | findstr 8080
```