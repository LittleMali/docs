# rpm-yum软件包
rpm是一套软件包管理工具，rpm通常用于centos系统，与dpkg-apt类似，rpm是底层包管理工具，yum是上层实现，负责处理依赖等其他细节。

## 包管理命令
### ### 查看已安装
```shell
$ rpm -qa --queryformat '%{NAME};%{VERSION};%{INSTALLTIME:date};%{VENDOR};%{SUMMARY}\n'
zlib;1.2.7;2023年10月27日 星期五 02时47分05秒;CentOS;The compression and decompression library

$ yum list 
zip.x86_64	3.0-11.el7		@anaconda
zlib.x86_64	1.2.7-17.el7	@anaconda
```

### 查看可升级
```shell
$ sudo yum list updates # 推荐，好解析
# 中间的版本号是可以升级到的版本号
cheese-libs.x86_64        2:3.28.0-1.el7                  base
chkconfig.x86_64          1.7.6-1.el7                     base

$ sudo yum check-update
checkpolicy.x86_64        2.5-8.el7                       base
cheese.x86_64             2:3.28.0-1.el7                  base
取代的软件包
NetworkManager.x86_64         1:1.18.8-2.el7_9            updates
    NetworkManager.x86_64     1:1.4.0-12.el7              @anaconda
```

## 版本号规范
### 版本号命令
RPM包的命名规则通常遵循以下格式：  
`<name>-<version>-<release>.<disttag>.<architecture>.rpm`

例如，iwl100-firmware-39.31.5.1-81.el7_9.noarch.rpm：
* name: iwl100-firmware - 软件包名称。
* version: 39.31.5.1 - 软件包的上游版本号。
* release: 81 - 发行版的修订号，通常表示针对特定发行版的构建或补丁。
* disttag: el7_9 - 发行版标识，表示该包是为哪个发行版构建的（在这里是RHEL/CentOS 7.9）。
* architecture: noarch - 软件包的目标架构，noarch表示该包适用于任何架构。

这种命名规则在大多数基于RPM的发行版（如RHEL、CentOS、Fedora等）中都是通用的。然而，不同的发行版可能会有一些细微的差别，例如在disttag部分使用不同的标识符。具体的命名规则和约定可能因发行版而异，但通常都遵循上述基本格式。

### 版本号比较
* 源码： https://github.com/rpm-software-management/rpm/blob/master/rpmio/rpmvercmp.c
  * 函数： int rpmvercmp(const char * a, const char * b)
* 文档： https://docs.fedoraproject.org/en-US/packaging-guidelines/Versioning/