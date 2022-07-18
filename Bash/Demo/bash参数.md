# bash参数

## 简单参数
简单参数是指命令行参数严格按照顺序输入，包括中间的空格，例如，`test.sh a b c`，
参数a、b、c按序输入，中间不允许跳过。通常这种方式适合参数固定，使用简单的场景。

### 简单参数例子
```bash
#!/bin/bash

PWD=$1
TEST_FILE=$2
DELETE_CONFIG=$3

# 参数校验
if [[ -z $PWD ]]; then
    echo "empty param: pwd"
    exit 1
fi

if [[ -z $TEST_FILE ]]; then
    echo "empty param: test path"
    exit 1
fi

if [[ ! -f "$TEST_FILE" ]]; then
    echo "test path is not exits"
    exit 1
fi

# 开始干活
delCfg=0 # default
if [[ $DELETE_CONFIG = "delcfg" ]]; then
    echo "del cfg"
    delCfg=1
fi

if [[ $delCfg -eq 1 ]]; then
    # bash xxx delcfg
else
    # bash xxx delcfg
fi
```

## 格式化参数
在前面的例子中，简单参数无法跳过中间某个参数。格式化参数，在输入和解析命令行的时候提供了更多选择。

### 格式化参数例子
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
使用方法如下。
```bash
./bash_test.sh -d dd -D -m MM -f FF -t TT -x XX
```
* 在 d 后面加“引号:” ，表示我们要求-d 传参。
* 在 D 后面没有加“引号:”，表示我们仅仅需要-D即可，-D类似于开关选项true or false。
* 我们希望解析的参数全面列在了optstring里面（d:Dm:f:t:），那么，我们在最后多传入了-x XX，getopts会报错 Illegal option -x ，但是参数解析还是可以正常完成。如果想要忽略这个错误，可以在 optstrings 最开头加上 引号（:d:Dm:f:t:），这是告诉getopts忽略未知的参数。

### getopts
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