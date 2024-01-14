# dpkg-apt软件包
dpkg-apt软件包管理是Debian系统的包管理工具。
dpkg是软件包管理工具，负责软件的安装卸载，apt是dpkg的上层封装，apt通过dpkg来管理软件包，同时apt还负责处理软件包之间的依赖性。

## 包管理命令
### 查看已安装
```shell
#  推荐，更全，好解析
$ dpkg -l | grep sqlit
ii  sqlite3   3.31.1-4kylin0.2    amd64   Command line interface for SQLite 3

$ apt list --installed | grep sqlit
sqlite3/10.1,now 3.31.1-4kylin0.2 amd64 [已安装，可升级至：3.31.1-4kylin0.3]
```

apt不会显示依赖的包，dpkg会显示所有已安装的包，所以，两者的数量是不相同的。
```shell
$ dpkg -l | wc -l
1888

$ apt list --installed | wc -l

WARNING: apt does not have a stable CLI interface. Use with caution in scripts.

1884
```

### 查看可升级
```shell
$ sudo apt update
$ sudo apt list --upgradable | grep sqlit
sqlite3/10.1-2203-hwe-updates 3.31.1-4kylin0.3 amd64 [可从该版本升级：3.31.1-4kylin0.2]
```
每个字段解读如下。
1. sqlite3：软件名。
2. 10.1-2203-hwe-updates：软件包来源的存储库或者分支。
3. 3.31.1-4kylin0.3：新版本。
4. amd64：架构。
5. 3.31.1-4kylin0.2：当前版本。

### 安装指定版本
```shell
sudo apt install <package-name>=<version>
sudo apt install xwayland=2:1.20.13-1ubuntu1~20.04.9
```

## 版本号规范
### 版本号定义
Debian包管理系统，其软件包（.deb文件）的命名规则通常遵循以下格式：  
`<package_name>_<version_number>-<package_revision>_<architecture>.deb`

在这个格式中：
* package_name：这是软件包的名称。
* version_number：这是软件包的上游版本号。
* package_revision：这是针对特定发行版的修订号，通常表示针对特定发行版的构建或补丁。
* architecture：这是软件包的目标架构，例如amd64、i386等。对于无关架构的包，这里通常是all。

这种命名规则在所有使用Debian包管理系统的发行版中都是通用的，包括Debian、Ubuntu、Linux Mint等。

例如，一个名为nginx_1.14.2-2+ubuntu18.04.1+deb.sury.org+1_amd64.deb的包：  
* nginx 是包名。  
* 1.14.2-2+ubuntu18.04.1+deb.sury.org+1 是版本号和修订号。  
* amd64 是目标架构。  

<package_revision>，也被称为Debian修订版号，是一个用于表示针对特定Debian或Ubuntu发行版的软件包修订或构建的字段。这个字段通常由一个或多个部分组成，这些部分由加号（+）或冒号（:）分隔。

在Debian和Ubuntu的软件包中，<package_revision>通常遵循以下格式：  
`<debian_revision>ubuntu<ubuntu_revision>`

其中：  
* <debian_revision>：这是Debian的修订版号。这个数字表示自从软件包的上游版本发布以来，Debian维护者对软件包所做的修订次数。
* <ubuntu_revision>：这是Ubuntu的修订版号。这个数字表示自从软件包被导入到Ubuntu以来，Ubuntu维护者对软件包所做的修订次数。

例如，在1.14.2-2+ubuntu18.04.1+deb.sury.org+1这个版本号中，2是Debian修订版号，ubuntu18.04.1表示这是针对Ubuntu 18.04的第一次修订。

上面的说法是一个标准的格式，但是很多软件的命名乱七八糟，最难处理的是version_number和package_revision，这两个部分的划分并不是很明确。再加上各种渠道、分支差异，试图人工解读并不现实。但是，很多时候我们试图解读版本号的目的是比较版本号大小。

### 版本号大小比较
* 文档：https://www.debian.org/doc/debian-policy/ch-controlfields.html#version
* 命令：`dpkg --compare-versions`
* 算法源码：https://github.com/guillemj/dpkg/blob/main/lib/dpkg/deb-version.c

```
dpkg --compare-versions 1.0.0-1 lt 1.0.1-1 && echo "1.0.0-1 is less than 1.0.1-1"

lt表示"less than"（小于）
gt（大于），le（小于或等于），ge（大于或等于），eq（等于）和ne（不等于）
```
`dpkg --compare-versions`的源码在dpkg可以找到，应该可以借鉴一下，没有实际剥离过。
* 源码1：https://github.com/guillemj/dpkg/blob/main/lib/dpkg/deb-version.c
  * 函数： deb_version_parse
* 源码2：https://github.com/guillemj/dpkg/blob/main/lib/dpkg/parsehelp.c
  * 函数：parseversion

