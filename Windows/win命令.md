# win命令

## 基础
* 打开服务管理界面  
  services.msc

* 查看服务状态  
方法1：运行services.msc。  
方法2：`sc query`

* 查看驱动程序  
  `sc query type=driver | findstr "xxx"`
  `sc query xxx`

* 删除驱动
  `sc delete xxx`

* 安装驱动
  `sc create twpcap type=kernel binpath="C:\xxx.sys"`

* 启动服务
  `sc start xxx`


* sc命令  
SC 是用来与服务控制管理器和服务进行通信的命令行程序。下面仅列举了查询相关的参数。
```
QUERY 和 QUERYEX 选项:
        如果查询命令带服务名称，将返回
        该服务的状态。其他选项不适合这种
        情况。如果查询命令不带参数或
        带下列选项之一，将枚举此服务。
    type=    要枚举的服务的类型(driver, service, userservice, all)
             (默认 = service)
    state=   要枚举的服务的状态 (inactive, all)
             (默认 = active)
    bufsize= 枚举缓冲区的大小(以字节计)
             (默认 = 4096)
    ri=      开始枚举的恢复索引号
             (默认 = 0)
    group=   要枚举的服务组
             (默认 = all groups)

语法示例
sc query                - 枚举活动服务和驱动程序的状态
sc query eventlog       - 显示 eventlog 服务的状态
sc queryex eventlog     - 显示 eventlog 服务的扩展状态
sc query type= driver   - 仅枚举活动驱动程序
sc query type= service  - 仅枚举 Win32 服务
sc query state= all     - 枚举所有服务和驱动程序
sc query bufsize= 50    - 枚举缓冲区为 50 字节
sc query ri= 14         - 枚举时恢复索引 = 14
sc queryex group= ""    - 枚举不在组内的活动服务
sc query type= interact - 枚举所有不活动服务
sc query type= driver group= NDIS     - 枚举所有 NDIS 驱动程序
```

* 驱动运行状态
`driverquery /V > c:\driver.txt`