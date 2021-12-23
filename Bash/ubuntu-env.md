# ubuntu

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
sudo apt-get install build-essential
2. 安装OpenGL Library
sudo apt-get install libgl1-mesa-dev
3. 安装OpenGL Utilities
sudo apt-get install libglu1-mesa-dev
4. 安装OpenGL Utility ToolKit
sudo apt-get install freeglut3-dev

## 添加环境变量
安装完qtcreator后，shell识别不了qmake命令，我们需要把qmake添加到系统环境变量。
1. 修改/etc/enviroment文件
   全局，用久生效。
```shell
#打开/etc/environment文件，其内容如下：
#在环境变量PATH中添加你要添加的路径即可，冒号作为分隔
PATH="/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games" 
```
2. 修改/etc/profile文件
   全局，用久生效。
   这个文件不是保存环境变量信息的文件，在登录时，系统只是根据它的内容对环境变量进行设置。
```shell
#其中，各个PATH之间用冒号分隔，$PATH指代添加your path前的环境变量。
export PATH=$PATH:[your path1]:[your path2]:[...]  
export PATH=[your path1]:[your path2]:[...]:$PATH 
```
3. 修改~/.profile文件
   当前用户作用，用久生效。
   这个文件的作用与方法2的文件相同，唯一的差别就是作用域的区别。
4. 修改/etc/bashrc
   全局，用久生效。
   这个文件同样不是为了保存环境变量，只是在使用bash shell时进行设置而已，所以设置和方法2中一样。
   这个文件只是对bash shell的环境变量的设置，脱离的bash shell，不使用命令行了，这样的修改就可能没有意义了。
```shell
#其中，各个PATH之间用冒号分隔，$PATH指代添加your path前的环境变量。
export PATH=$PATH:[your path1]:[your path2]:[...]  
export PATH=[your path1]:[your path2]:[...]:$PATH 
```
5. 修改～/.bashrc
   当前用户，用久生效。
   这个文件的作用与方法4的文件相同，唯一的差别就是作用域的区别。
6. 在Terminal中使用shell命令
   只在当前Terminal中起作用，关闭了当前Terminal就无效了。对其他Terminal也无效。
```shell
#在Bash中直接用下面命令：
export PATH=$PATH:[your path1]:[your path2]:[...]  
export PATH=[your path1]:[your path2]:[...]:$PATH
```

上面的修改方法，除了最后一种外，并不会立刻生效，需要重启或者重新登录才行。如果想要立即生效，可以调用下面的方法。
```shell
source [file name]  # file name 指的是上面修改过的文件的file name
```

### 显示环境变量
```shell
env #打印所有的环境变量
echo $PATH #打印PATH环境变量  
```