# win生成dmp文件

windbg attach到进程上，执行命令 `.dump -ma test.dmp`。

微软：[windbg生成dmp文件命令](https://learn.microsoft.com/en-us/windows-hardware/drivers/debugger/-dump--create-dump-file-)

* -m ：mini dmp
* -f ：full dmp
* -h ：句柄信息。
* -ma ： mini dmp with all options。使用这个命令导出dmp。  
在user态下，优先使用 -ma 命令导出dmp，它比 -f 导出的信息要更全。  
-ma 的 a，可以理解为all。  

在一篇文章中提到了，win的任务管理器导出的dmp，默认是不包括 -h 的，也就是不包括句柄信息。  
使用procexp导出的dump是可以看到句柄信息的。

