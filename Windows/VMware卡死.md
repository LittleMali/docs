# VMWare运行一段时间后卡死

## 现象
打开虚拟机后可以正常运行，运行大约20分钟后突然卡死，风扇急速转动，然后通过任务管理器可以观察到VMware Workstation VMX应用程序的CPU占比高达80%,CPU的利用率增加至100%。使用任务管理器杀死VMware Workstation VMX程序后，CPU利用率就马上下降至恢复正常范围。

## 解决
1. 检查Hyper-V没有开启。
![20230228105316](https://raw.githubusercontent.com/LittleMali/docs/master/mdPics/20230228105316.png)
![20230228105333](https://raw.githubusercontent.com/LittleMali/docs/master/mdPics/20230228105333.png)
![20230228105341](https://raw.githubusercontent.com/LittleMali/docs/master/mdPics/20230228105341.png)
确定Hyper-V如图所示，没有勾选。如果勾选了，就取消勾选后，确定，重启电脑即可。

2. 禁止HV主机服务。
打开服务services.msc。
![20230228105438](https://raw.githubusercontent.com/LittleMali/docs/master/mdPics/20230228105438.png)
![20230228105449](https://raw.githubusercontent.com/LittleMali/docs/master/mdPics/20230228105449.png)

命令行输入： `bcdedit /set hypervisorlaunchtype off`
