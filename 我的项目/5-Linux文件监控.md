# Linux文件监控

Linux内核提供几个监控机制有inotify，fanotify，audit。其中inotify和fanotify主要是文件系统的监控，也就是监控文件访问，audit是系统调用的审核服务，也支持文件监控。

## inotify
Linux内核自2.6.13版本开始引入，主要是监控文件操作。
* 访问（IN_ACCESS）：文件被访问（读取）。
* 修改（IN_MODIFY）：文件被修改。
* 属性改变（IN_ATTRIB）：文件的元数据（例如权限、时间戳、链接数等）被修改。
* 关闭写入（IN_CLOSE_WRITE）：可写文件被关闭。
* 关闭非写入（IN_CLOSE_NOWRITE）：不可写文件被关闭。
* 打开（IN_OPEN）：文件被打开。
* 移动（IN_MOVED_FROM, IN_MOVED_TO）：文件被移动或重命名。
* 创建（IN_CREATE）：在监控的目录中创建了新文件或新目录。
* 删除（IN_DELETE）：在监控的目录中删除了文件或目录。
* 自删除（IN_DELETE_SELF）：被监控的文件或目录被删除。
* 自移动（IN_MOVE_SELF）：被监控的文件或目录被移动。

注意：inotify最大的问题是：
1. inotify拿不到进程pid，也就是不知道是谁操作了文件。
2. inotify只能监控指定目录及该目录下的文件，无法递归的监控下级子目录！！！

所以，我们不使用inotify进行文件监控的原因就在此。

### inotify使用
如何判断系统是否支持inotify？
```shell
## 输出y表示支持
% grep INOTIFY_USER /boot/config-$(uname -r)
CONFIG_INOTIFY_USER=y
```

inotify 工具库提供监控文件系统活动的命令行工具。
```shell
$ apt-get install inotify-tools

## 该工具提供两个命令
## inotifywait 仅执行阻塞，等待 inotify 事件。
## 开启监控，
##  -r|--recursive	Watch directories recursively.
##  -m|--monitor  	Keep listening for events forever.  Without this option, inotifywait will exit after one。
$ inotifywait -rm /home/user/桌面/inotify-test

## 在文件夹中进行操作时，inotifywait会打印很多event事件。
$ inotifywait -rm /home/user/桌面/inotify-test
Setting up watches.  Beware: since -r was given, this may take a while!
Watches established.

## 这是ls命令的事件日志。
/home/user/桌面/inotify-test/ OPEN,ISDIR 
/home/user/桌面/inotify-test/ ACCESS,ISDIR 
/home/user/桌面/inotify-test/ CLOSE_NOWRITE,CLOSE,ISDIR 

# 这是 mv dir1/inner-dir/test2.txt ./yoyoy.txt 的事件日志。
/home/user/桌面/inotify-test/dir1/inner-dir/ MOVED_FROM test2.txt
/home/user/桌面/inotify-test/ MOVED_TO yoyoy.txt
/home/user/桌面/inotify-test/ OPEN yoyoy.txt
/home/user/桌面/inotify-test/ ACCESS yoyoy.txt
/home/user/桌面/inotify-test/ CLOSE_NOWRITE,CLOSE yoyoy.txt
/home/user/桌面/inotify-test/ OPEN yoyoy.txt
/home/user/桌面/inotify-test/ ACCESS yoyoy.txt
/home/user/桌面/inotify-test/ CLOSE_NOWRITE,CLOSE yoyoy.txt
/home/user/桌面/inotify-test/ OPEN yoyoy.txt
/home/user/桌面/inotify-test/ ACCESS yoyoy.txt
/home/user/桌面/inotify-test/ CLOSE_NOWRITE,CLOSE yoyoy.txt

## inotifywatch 收集关于被监视的文件系统的统计数据，包括每个 inotify 事件发生多少次。
$ inotifywatch -r /home/user/桌面/inotify-test
Establishing watches...
Finished establishing watches, now collecting statistics.

## 在文件夹里面进行各种操作，然后，退出inotifywatch。
total  access  modify  close_write  close_nowrite  open  filename
26     8       2       1            7              8     /home/user/桌面/inotify-test/
```


## fanotify
fanotify是inotify的替代版，从内存2.6.36版本引入。相对inotify而言，fanotify的优势有：
* inotify只能监控指定目录及其下的文件变化，无法监控子目录的变化，fanotify稍微进步了一些，可以监控指定目录及其下一级的子目录，如果想再往下一级目录那就不行了。
* fanotify提供了global模式，可以监控全盘的文件变化，也提供了per-mount模式，可以监控某个挂载点上所有文件的变化。这算是一种变通，我们可以通过过滤的方式来忽略掉我们不感兴趣的文件从而达到 递归监控整个目录的效果。
* 当然，使用inotify和fanotify都可以用代码实现监控，递归遍历出所有的子目录然后一个一个监控。
* inotify只能知道某个文件被操作了，但是，它不知道是谁操作的，也就是说inotify拿不到pid，但是，fanotify可以。
* inotify是旁路监听，fanotify可以对文件操作进行阻断，当一个文件被打开或运行时，系统会阻塞当前进程的调用，直到fanotify写入allow/deny结果，才会恢复运行。