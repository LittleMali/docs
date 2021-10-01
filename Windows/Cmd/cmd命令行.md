# win Cmd命令行

# 命令行传参
使用命令行传参时，遇到特殊符号需要转义。
比如如下的命令行，我们试图传参拉起exe，但是缺提示k2不是内部或外部命令，这是因为k1和k2中间的“&”对于cmd而言是无法直接处理的，需要我们显示转义才行。
![20211001143253](https://raw.githubusercontent.com/LittleMali/docs/master/mdPics/20211001143253.png)

![20211001143426](https://raw.githubusercontent.com/LittleMali/docs/master/mdPics/20211001143426.png)

这里仅仅是记录一下转义字符，方便以后命令行传参，并不详细讨论cmd是如何解析命令行的，网上简单查过，规则还是比较复杂的。
对于上面例子，应该这么写：
```
"runexe.exe" /S ##k1=v1^&k2=v2
```
