# win服务启动顺序

## 启动顺序
https://learn.microsoft.com/en-us/windows/win32/services/automatically-starting-services

During system boot, the SCM starts all auto-start services and the services on which they depend. For example, if an auto-start service depends on a demand-start service, the demand-start service is also started automatically.

The load order is determined by the following:

1. The order of groups in the load ordering group list. This information is stored in the List value in the following registry key:

*HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\ServiceGroupOrder*

To specify the load ordering group for a service, use the lpLoadOrderGroup parameter of the CreateService or ChangeServiceConfig function.

2. The order of services within a group specified in the tags order vector. This information is stored in the following registry key:

*HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\GroupOrderList*

3. The dependencies listed for each service.

翻译过来就是说 先按组顺序启动，其次，在每个组内部按照tag order顺序启动，同时，还要考虑服务之间的依赖关系，谁被人依赖了， 谁要先起来。

## 调整顺序

```c
BOOL ChangeServiceConfigA(
  [in]            SC_HANDLE hService,
  [in]            DWORD     dwServiceType,
  [in]            DWORD     dwStartType,
  [in]            DWORD     dwErrorControl,
  [in, optional]  LPCSTR    lpBinaryPathName,
  [in, optional]  LPCSTR    lpLoadOrderGroup,
  [out, optional] LPDWORD   lpdwTagId,
  [in, optional]  LPCSTR    lpDependencies,
  [in, optional]  LPCSTR    lpServiceStartName,
  [in, optional]  LPCSTR    lpPassword,
  [in, optional]  LPCSTR    lpDisplayName
);
```

## lpLoadOrderGroup

The name of the load ordering group of which this service is a member. Specify NULL if you are not changing the existing group. Specify an empty string if the service does not belong to a group.

The startup program uses load ordering groups to load groups of services in a specified order with respect to the other groups. The list of load ordering groups is contained in the ServiceGroupOrder value of the following registry key:

HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control

下面的值就是从本机win10注册表拷贝出来的。

```
System Reserved
EMS
WdfLoadGroup
Boot Bus Extender
System Bus Extender
SCSI miniport
Port
Primary Disk
SCSI Class
SCSI CDROM Class
FSFilter Infrastructure
FSFilter System
FSFilter Bottom
FSFilter Copy Protection
FSFilter Security Enhancer
FSFilter Open File
FSFilter Physical Quota Management
FSFilter Virtualization
FSFilter Encryption
FSFilter Compression
FSFilter Imaging
FSFilter HSM
FSFilter Cluster File System
FSFilter System Recovery
FSFilter Quota Management
FSFilter Content Screener
FSFilter Continuous Backup
FSFilter Replication
FSFilter Anti-Virus
FSFilter Undelete
FSFilter Activity Monitor
FSFilter Top
Filter
Boot File System
Base
Pointer Port
Keyboard Port
Pointer Class
Keyboard Class
Video Init
Video
Video Save
File System
Streams Drivers
NDIS Wrapper
COM Infrastructure
Event Log
PerceptionGroup
ProfSvc_Group
AudioGroup
UIGroup
MS_WindowsLocalValidation
PlugPlay
Cryptography
PNP_TDI
NDIS
TDI
iSCSI
NetBIOSGroup
ShellSvcGroup
SchedulerGroup
SpoolerGroup
SmartCardGroup
NetworkProvider
MS_WindowsRemoteValidation
NetDDEGroup
Parallel arbitrator
Extended Base
PCI Configuration
MS Transactions
```