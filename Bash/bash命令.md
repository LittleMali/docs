# Bash命令

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