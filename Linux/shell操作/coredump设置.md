# dmp
## core dump是否开启
ulimit -a
* 查看core file size，若为0则表示没有开启。
* 可以用过ulimit -c unlimited来设置。
* 上面的命令只对当前的终端环境生效，想要永久生效，可以修改 /etc/security/limits.conf 。增加一行。
![20220311160604](https://raw.githubusercontent.com/LittleMali/docs/master/mdPics/20220311160604.png)

## 查询core dump的路径
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

## core dump位置
在有的linux系统下，查看core dmp的结果如下，这是系统使用了systemd-coredump来生成dmp文件。
```
$cat /proc/sys/kernel/core_pattern
|/lib/systemd/systemd-coredump %P %u %g %s %t 9223372036854775808 %h %e
```
systemd-coredump默认生成的路径是`/var/lib/systemd/coredump`，默认是LZ4压缩存放的。
我们利用coredumpctl命令可解压。

如何解压lz4格式的文件？  
`coredumpctl -o myexe.dmp dump 1234`  
-o输出文件到哪里。  
dump后面是pid，pid是发生crash的那个进程，pid可以在lz4的文件名中看到。

## 配置core-dump
```
echo "kernel.core_pattern = /tmp/core-%p-%e-%t">>/etc/sysctl.conf
echo "kernel.core_uses_pid = 1">>/etc/sysctl.conf
echo "fs.suid_dumpable = 2">>/etc/sysctl.conf
sysctl -p
echo "DefaultLimitCORE=infinity">>/etc/systemd/system.conf

systemctl daemon-reexec
```