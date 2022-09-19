# bash路径操作

## 获取全路径
在脚本内部，获取项目所在的全路径。
```
# 文件目录结构
xxxSln
|_ scripts
    |_ build.sh
```

在build.sh里获取xxxSln的目录。
``` bash
#!/bin/bash
WORKSPACE=$(cd `dirname $0`; cd ../; pwd)
echo workspace=${WORKSPACE}
```

## 获取桌面路径
linux下有个配置`config/user-dirs.dirs`记录了路径。
```
# ~/.config/user-dirs.dirs
XDG_DESKTOP_DIR="$HOME/Desktop"
XDG_DOWNLOAD_DIR="$HOME/Downloads"
XDG_TEMPLATES_DIR="$HOME/Templates"
XDG_PUBLICSHARE_DIR="$HOME/Public"
XDG_DOCUMENTS_DIR="$HOME/Documents"
XDG_MUSIC_DIR="$HOME/Music"
XDG_PICTURES_DIR="$HOME/Pictures"
XDG_VIDEOS_DIR="$HOME/Videos"
```
shell脚本获取桌面路径。
```bash
desk=`cat $HOME/.config/user-dirs.dirs | grep DESKTOP | tail  -1  |cut -d '=' -f 2  | sed 's/\"//g'`
var=`eval echo $desk`
echo $var
```