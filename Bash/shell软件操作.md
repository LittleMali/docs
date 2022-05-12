# shell命令

## 软件
### apt-get
```shell
sudo apt-get install XXX
sudo apt-get install -y XXX // 不用确认
sudo apt-get install -q XXX // 静默
sudo apt-get remove XXX // 仅卸载软件，不卸载配置文件
sudo apt-get purge XXX // 同时卸载相应的配置文件
sudo apt-get update // 将所有包的来源更新
sudo apt-get upgrade // 将系统中旧版本的包升级成最新的
```

# apt
* 查看已安装的包  
  `apt list --installed`
  `dpkg -l`

* 查看安装软件包的路径  
  `dpkg -L <package-name>`

* 卸载  
  `apt-get remove <package-name>`

* 查看可以安装的版本list  
  `apt-cache madison soft_name`

* 安装指定的版本  
  `apt-get install <package-name>=version`

* 查看包依赖  
  查看curl依赖哪些包。  
  `apt-cache depends curl`

* 查看包的说明
  `apt-cache show libcurl3`

### dpkg
```shell
dpkg -l --列出当前系统中所有的包.可以和参数less一起使用在分屏查看. (类似于rpm -qa)
dpkg -l |grep -i "软件包名" --查看系统中与"软件包名"相关联的包.
dpkg -s 查询已安装的包的详细信息.
dpkg -L 查询系统中已安装的软件包所安装的位置. (类似于rpm -ql)
dpkg -S 查询系统中某个文件属于哪个软件包. (类似于rpm -qf)
dpkg -I 查询deb包的详细信息,在一个软件包下载到本地之后看看用不用安装(看一下呗).
dpkg -i 手动安装软件包
dpkg -r 卸载软件包.不是完全的卸载,它的配置文件还存在.
dpkg -P 全部卸载(但是还是不能解决软件包的依赖性的问题)
```

## 安装OpenGL
1. 建立基本编译环境  
`sudo apt-get install build-essential`
2. 安装OpenGL Library  
`sudo apt-get install libgl1-mesa-dev`
3. 安装OpenGL Utilities  
`sudo apt-get install libglu1-mesa-dev`
4. 安装OpenGL Utility ToolKit  
`sudo apt-get install freeglut3-dev`
