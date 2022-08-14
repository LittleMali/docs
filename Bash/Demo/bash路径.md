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