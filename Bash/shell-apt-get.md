# apt

* 查看已安装的包
  apt list --installed
  dpkg -l

* 查看安装软件包的路径
  dpkg -L <package-name>

* 卸载
  apt-get remove <package-name>

* 查看可以安装的版本list
  apt-cache madison soft_name

* 安装指定的版本
  apt-get install <package-name>=version

  