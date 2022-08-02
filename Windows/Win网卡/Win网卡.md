# Windows网卡信息

1. 如何获取本机网卡信息？  
    * 函数：GetAdaptersInfo。
    * 能获取到所有网卡的信息，物理网卡，虚拟网卡，包括已断开连接的网卡（比如有线网卡没有插光纤）。
2. 如何区分物理网卡和虚拟网卡呢？
    * 本机安装了多个虚拟机，那么，虚拟机会有一个虚拟网卡。
    * 通过 GetAdaptersInfo 没有办法直接区分，要配合注册表里的网卡信息。
    * 方法1：判断MediaSubType和PnpInstanceID组合。自测发现在虚拟机内使用，不大准确。
    * 方法2：判断Characteristics值。这个是准确的。

## 方法1：MediaSubType和PnpInstanceID
1. 通过 `GetAdaptersInfo` 枚举网卡信息，获取ip和adapter name。
2. 通过adapter name在注册表中查找对应网卡的`MediaSubType`和`PnpInstanceID`。  
```
HKEY_LOCAL_MACHINE
{AdapterName}要替换为获取到的设备名。
System\\CurrentControlSet\\Control\\Network\\{4D36E972-E325-11CE-BFC1-08002BE10318}\{AdapterName}\\Connection | MediaSubType
System\\CurrentControlSet\\Control\\Network\\{4D36E972-E325-11CE-BFC1-08002BE10318}\{AdapterName}\\Connection | PnpInstanceID
```
3. 判断结果。
    * 如果MediaSubType = 1，且PnpInstanceID前面以PCI字样开头则表示是本地实际网卡。
    * 如果MediaSubType = 1，且PnpInstanceID前面不以PCI字样开头则表示是本地虚拟网卡。
    * 如果MediaSubType = 2，则表示是无线网卡。

## 方法2：Characteristics
1. 遍历网卡注册表。
```
HKEY_LOCAL_MACHINE/SYSTEM/CurrentControlSet/Control/Class/{4D36E972-E325-11CE-BFC1-08002bE10318}/{连接索引号} | Characteristics
```
2. 判断`Characteristics`结果。  

值|定义|说明
--|--|--
0x1|NCF_VIRTUAL|说明组件是个虚拟适配器
0x2|NCF_SOFTWARE_ENUMERATED|说明组件是一个软件模拟的适配器
0x4|NCF_PHYSICAL|说明组件是一个物理适配器
0x8|NCF_HIDDEN|说明组件不显示用户接口
0x10|NCF_NO_SERVICE|说明组件没有相关的服务(设备驱动程序)
0x20|NCF_NOT_USER_REMOVABLE|说明不能被用户删除(例如，通过控制面板或设备管理器)
0x40|NCF_MULTIPORT_INSTANCED_ADAPTER|说明组件有多个端口，每个端口作为单独的设备安装。每个端口有自己的hw_id(组件ID) 并可被单独安装，这只适合于EISA适配器
0x80|NCF_HAS_UI|说明组件支持用户接口(例如，Advanced Page或Customer Properties Sheet)
0x400|NCF_FILTER|说明组件是一个过滤器

3. 注意，判断结果要用 `&` ，不要直接 `==`。  
如果是虚拟网卡：`Characteristics & NCF_VIRTUAL ==NCF_VIRTUAL`。  
如果是物理网卡：`Characteristics & NCF_PHYSICAL ==NCF_PHYSICAL`。