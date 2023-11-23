# Linux软件包

## 查看已安装的软件list
* deb包  
  `dpkg -l`  
  `apt list --installed`  
  apt不会显示依赖的包，dpkg会显示所有已安装的包，所以，两者的数量是不相同的。
* rpm包  
 ` yum list installed`

 
## 查看包信息命令

  

* 获取最新的软件包信息
  `apt update`

* 查看可更新的包
  `apt list --upgradable`
  `sudo yum check-update`
* 更新某个指定的包
  `sudo apt install <package-name>=<version>`
  `sudo apt install xwayland=2:1.20.13-1ubuntu1~20.04.9`
* 更新全部的包
  `apt upgrade`


麒麟官网-漏洞信息
https://www.kylinos.cn/support/loophole/patch.html

Ubuntu漏洞官网

https://ubuntu.com/security/notices


