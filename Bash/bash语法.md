# Bash命令

## 基础语法
if-else多层写法， ^是续写的意思。
```bash
set option=2
set sum=-1
if %option% == 3 ( 
  echo three 
  set /a sum=%option%*%option%*%option%
) ^
else if %option% == 2 ( 
  echo two 
  set /a sum=2*%option%
) ^
else if %option% == 1 ( 
  echo one 
  set /a sum=%option% 
) ^
else ( 
  echo zero 
  set /a sum=0 
)
echo sum = !sum!
```


## 字符串查找
在一个字符串变量中查找一个子串。
``` bash
set wholeTxt=I hava a apple
set keyWord= apple
echo %wholeTxt% | findstr %keyWord% > nul && echo yes || echo no
```

上面的例子会打印出一个结果：yes或者no，但是有时候我们想做更多的一些事情，可以这么来实现。
``` bash
set wholeTxt=I hava a apple
set keyWord= apple
set found=0
echo %wholeTxt% | findstr %keyWord% > nul && (
    set found=1
) || (
    set found=0
)

if %found% equ 1 (
    echo we find apple
)
```

``` bash
set wholeTxt=I hava a apple
set keyWord= apple
set found=0
echo %wholeTxt% | findstr %keyWord% > nul 
if %errorlevel% equ 0 (
    echo yes
) else (
    echo no
)
```

## 命令行传参
使用命令行传参时，遇到特殊符号需要转义。  
比如如下的命令行，我们试图传参拉起exe，但是缺提示k2不是内部或外部命令，这是因为k1和k2中间的“&”对于cmd而言是无法直接处理的，需要我们显示转义才行。  
![20211001143253](https://raw.githubusercontent.com/LittleMali/docs/master/mdPics/20211001143253.png)

![20211001143426](https://raw.githubusercontent.com/LittleMali/docs/master/mdPics/20211001143426.png)

这里仅仅是记录一下转义字符，方便以后命令行传参，并不详细讨论cmd是如何解析命令行的，网上简单查过，规则还是比较复杂的。  
对于上面例子，应该这么写：
```
"runexe.exe" /S ##k1=v1^&k2=v2
```

## win cmd切换硬盘
假设当前在C盘，直接输入D:即可。
```bash
D:
```