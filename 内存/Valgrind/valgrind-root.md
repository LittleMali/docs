# Valgrind Permission Denied

使用Valgrind分析root进程时，提示Permission denied。
```
$ sudo valgrind --tool=massif /opt/apps/my_file
==16660== 
==16660== Warning: Can't execute setuid/setgid/setcap executable: /opt/apps/my_file
==16660== Possible workaround: remove --trace-children=yes, if in effect
==16660== 
valgrind: /opt/apps/my_file: Permission denied
```
已经是sudo运行valgrind了，还是提示没权限。即使切换到root账号再运行，依旧提示没权限。

```
ll /opt/apps/my_file
-rwsr-sr-x 1 root root 166240 1月  23 10:16  /opt/apps/my_file
```
我们看到my_file有s权限。

我们移除s权限再继续使用valgrind，valgrind正常启动。
```
sudo chmod u-s,g-s /opt/apps/my_file
sudo $ sudo valgrind --tool=massif /opt/apps/my_file
```

导致Permission denied的核心因素是my_file设置了s权限。

## 文件权限s
`-rwsr-sr-x 1 root root`

这个文件的属性-rwsr-sr-x可以分解为四部分来解读：
1. 第一个字符-表示这是一个普通文件。如果是目录，这里会是d；如果是链接，这里会是l。
2. 接下来的三个字符rws表示文件所有者的权限。r表示可读，w表示可写，s表示setuid位已经被设置。当setuid位被设置时，任何用户运行这个程序，该程序都会以文件所有者的身份运行。
3. 接下来的三个字符r-s表示文件所属组的权限。r表示可读，-表示不可写，s表示setgid位已经被设置。当setgid位被设置时，任何用户运行这个程序，该程序都会以文件所属组的身份运行。
4. 最后的三个字符r-x表示其他用户的权限。r表示可读，-表示不可写，x表示可执行。

所以，-rwsr-sr-x表示这是一个可读、可写、可执行的文件，当任何用户运行这个程序时，该程序都会以文件所有者和文件所属组的身份运行。

当一个文件的权限被设置为-rwsr-sr-x，并且所有者和所属组都是root时，无论是谁（无论是普通用户还是通过sudo运行），只要运行这个程序，它都会以root用户的权限来运行。

这是因为setuid（表示为s）和setgid位被设置了。setuid位意味着当这个程序被执行时，它运行的权限将会变成它的所有者的权限，而不是运行用户的权限。同样，setgid位意味着当这个程序被执行时，它的组权限将会变成文件所属组的权限。

## 文件权限x
* s（setuid/setgid）：当s位被设置时，执行该文件的用户或组会临时获得该文件所有者或所属组的权限。例如，如果一个文件的所有者是root，并且设置了setuid位（如-rws------），那么无论谁（无论是普通用户还是通过sudo运行）运行这个程序，它都会以root用户的权限来运行。
* x（可执行）：x位表示用户、组或其他人是否有权限执行该文件。如果文件是一个程序或脚本，那么x位决定了谁可以运行它。但是，运行程序的权限级别仍然是运行用户的权限级别，除非设置了s位。

如果是 `-rwsr-sr-x 1 root root`，无论是sudo还是直接运行，它都会以root用户的权限来运行。
如果是 `-rwsr-sr-x 1 user user`，无论是sudo还是直接运行，它都会以user用户的权限来运行。
如果是 `-rwxr-xr-x 1 user user`，sudo运行则是root权限，直接运行就是user权限。