# Linux软件包

## 包管理命令
### 查看已安装的软件list
#### deb包  
  `apt list --installed | grep sqlit`  
  `dpkg -l | grep sqlit` (推荐，更全，好解析)
```
$ apt list --installed | grep sqlit
sqlite3/10.1,now 3.31.1-4kylin0.2 amd64 [已安装，可升级至：3.31.1-4kylin0.3]

$ dpkg -l | grep sqlit
ii  sqlite3                                               3.31.1-4kylin0.2                   amd64        Command line interface for SQLite 3
```

  apt不会显示依赖的包，dpkg会显示所有已安装的包，所以，两者的数量是不相同的。
```
$ dpkg -l | wc -l
1888

$ apt list --installed | wc -l

WARNING: apt does not have a stable CLI interface. Use with caution in scripts.

1884
```
#### rpm包  
  `yum list installed`
```
$ rpm -qa --queryformat '%{NAME};%{VERSION};%{INSTALLTIME:date};%{VENDOR};%{SUMMARY}\n'
zlib;1.2.7;2023年10月27日 星期五 02时47分05秒;CentOS;The compression and decompression library

$ yum list 
zip.x86_64	3.0-11.el7		@anaconda
zlib.x86_64	1.2.7-17.el7	@anaconda
```


### 查看哪些软件包可以升级
#### deb包
```
$ sudo apt update
$ sudo apt list --upgradable | grep sqlit
sqlite3/10.1-2203-hwe-updates 3.31.1-4kylin0.3 amd64 [可从该版本升级：3.31.1-4kylin0.2]
```
每个字段解读如下。
1. sqlite3：软件名。
2. 10.1-2203-hwe-updates：软件包来源的存储库或者分支。
3. 3.31.1-4kylin0.3：新版本。
amd4. 64：架构。
5. 3.31.1-4kylin0.2：当前版本。

#### rpm  
  `yum list updates`（推荐，好解析）  
  `yum list updates xxx`  
  `yum check-update`  
  `yum check-update xxx`
```
$ sudo yum check-update
checkpolicy.x86_64                     2.5-8.el7                       base     
cheese.x86_64                          2:3.28.0-1.el7                  base     
取代的软件包
NetworkManager.x86_64                  1:1.18.8-2.el7_9                updates  
    NetworkManager.x86_64              1:1.4.0-12.el7                  @anaconda

$ sudo yum list updates
cheese-libs.x86_64                       2:3.28.0-1.el7                  base   
chkconfig.x86_64                         1.7.6-1.el7                     base 
# 中间的版本号是可以升级到的版本号
```
 
## 查看包信息命令

  

* 获取最新的软件包信息
  `apt update`

* 查看可更新的包  
  `apt list --upgradable`  
  `sudo yum check-update`  
  `sudo yum check-update xxx`
* 更新某个指定的包
  `sudo apt install <package-name>=<version>`
  `sudo apt install xwayland=2:1.20.13-1ubuntu1~20.04.9`
* 更新全部的包
  `apt upgrade`





麒麟官网-漏洞信息
https://www.kylinos.cn/support/loophole/patch.html

Ubuntu漏洞官网

https://ubuntu.com/security/notices


uos漏洞
https://src.uniontech.com/#/security_advisory?t=1701834644264
