# win服务

win服务进程是指在系统后台运行的一系列程序，其通常情况下很少需要跟用户进行交互。如果是在Unix系统上，服务进程一般叫daemon。

## 资料
[微软文档](https://learn.microsoft.com/en-us/windows/win32/services/services)

## 服务管理器
win系统通过服务管理器service control manager (SCM)来管理服务和驱动。
SCM内部使用一个数据库来存储服务信息，位于注册表的`HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services`下。  
服务分为两种：
* 开机自启服务（Automatically Starting Services）。
* 手动启动服务（Starting Services on Demand）。

## 服务程序
要编写一个服务程序，需要包括以下三个点：
* 服务入口（Service Entry Point）。
* 服务主程序（Service ServiceMain Function）。
* 服务控制函数（Service Control Handler Function）。

### 服务入口
