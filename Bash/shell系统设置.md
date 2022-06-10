# shell命令
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

## dmp
### core dump是否开启
ulimit -a
* 查看core file size，若为0则表示没有开启。
* 可以用过ulimit -c unlimited来设置。
* 上面的命令只对当前的终端环境生效，想要永久生效，可以修改 /etc/security/limits.conf 。增加一行。
![20220311160604](https://raw.githubusercontent.com/LittleMali/docs/master/mdPics/20220311160604.png)

### 查询core dump的路径
```bash
# 方法1：
$ cat /proc/sys/kernel/core_pattern

方法2：
$ /sbin/sysctl kernel.core_pattern
```
例如：cat /proc/sys/kernel/core_pattern 会输出如下：
/tmp/core-%p-%e-%t
* %e 程序文件名。
* %p 所dump进程的pid。
* %t core dump的时间 (由1970年1月1日计起的秒数)。
* %u 所dump进程的实际用户ID。 
* %g 所dump进程的实际组ID。
* %s 导致本次core dump的信号。

### 配置core-dump
```
echo "kernel.core_pattern = /tmp/core-%p-%e-%t">>/etc/sysctl.conf
echo "kernel.core_uses_pid = 1">>/etc/sysctl.conf
echo "fs.suid_dumpable = 2">>/etc/sysctl.conf
sysctl -p
echo "DefaultLimitCORE=infinity">>/etc/systemd/system.conf

systemctl daemon-reexec
```

