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

## 参数解析
### 简单的传参和解析参数
严格限定传参格式，参数的顺序和值不可修改。假设脚本调用 sh test.sh p1 p2 。
那么获取参数可以用 $1 , $2 。
这种方式比较简单，在参数较多并且有参数可以省略的情况下，这个方式不好。
```bash
#!/bin/bash
# sh test.sh p1 p2
echo "param0:" $0 # test.sh
echo "param1:" $1 # p1
echo "param2:" $2 # p2
echo "param3:" $3 # 空
if [ "$3" != "" ]; then
    echo $3
else
    echo "p3 is null"
fi
```
### 合理的传参和解析参数
利用*getopts*来解析参数，参数格式是 -u xx -d xxx 。
* getopts语法。
  getopts optstring name [args]
* 参数 optstring。
  1. 需要识别选项列表。说明哪些选项有效，以字母顺序列出。例如，字符串ht表示选项-h和-t有效。
  2. 如果字符后面跟一个 :, 表示该选项期望有一个参数，与选项以空白字符分割。
  3. ？ 和 ： 不能作为选项字符使用。
* 参数 name。
  一个变量，用来填充要处理的选项。
* 参数 args。
  是要处理的参数和选项的列表。如果未提供，则默认为提供给应用程序($@)的参数和选项。可以提供第三个参数，以使用getopts解析您提供的参数和选项的任何列表。

```bash
#!/bin/bash
while getopts d:Dm:f:t: OPT
do
    case "${OPT}" 
    in
        d) DEL_DAYS=${OPTARG};;
        D) DEL_ORIGINAL='yes';;
        f) DIR_FROM="$OPTARG";;
        m) MAILDIR_NAME="$OPTARG";;
        t) DIR_TO="$OPTARG";;
        ?) echo "unknown" ${OPTARG}
    esac
done
  
shift $(($OPTIND - 1))

echo $DEL_DAYS
echo $DEL_ORIGINAL
echo $DIR_FROM
```
在上面的例子中，我们可以这么调脚本。
sh bash_test.sh -d dd -D -m MM -f FF -t TT -x XX
* 在 d 后面加“引号:” ，表示我们要求-d 传参。
* 在 D 后面没有加“引号:”，表示我们仅仅需要-D即可，-D类似于开关选项true or false。
* 我们希望解析的参数全面列在了optstring里面（d:Dm:f:t:），那么，我们在最后多传入了-x XX，getopts会报错 Illegal option -x ，但是参数解析还是可以正常完成。如果想要忽略这个错误，可以在 optstrings 最开头加上 引号（:d:Dm:f:t:），这是告诉getopts忽略未知的参数。

## 文件操作
判断文件是否存在
```bash
if exists aa.txt (
    if not exists bb.exe (
        echo "xx.txt is exists"
    )
)

```
删除文件
```bash
if exists xx.txt (
    del xx.txt
)
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