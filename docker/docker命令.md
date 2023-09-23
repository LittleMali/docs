# docker命令

教程  
https://www.widuu.com/chinese_docker/userguide/dockerimages.html

* 查看docker镜像  
  `docker imgages`

* 查看docker容器  
  `docker ps`

* 启动/停止docker  
  `docker start <container_name/container_id>`  
  `docker stop <container_name/container_id>`  
  `docker restart <container_name/container_id>`

* 进入docker  
  `docker exec -it <容器名称或ID> /bin/bash`  
  `docker attach <容器名称>`  

* docker后台运行  
  `docker run -itd --name <>`

* 退出容器  
  `Ctrl+P`

* 导入/导出容器  
  `docker export <容器id> ubuntu.tar`  
  `cat ubuntu.tar | docker import - test/ubuntu:v1`

* docker删除  
  `docker rm -f <container_id>`

* docker映射的本地目录  
  `docker inspect <容器名称或ID> | grep -i "Mounts"`
```
"Mounts": [
    {
        "Type": "bind",
        "Source": "/root/ci_workspace/master",
        "Destination": "/data",
        "Mode": "",
        "RW": true,
        "Propagation": "rprivate"
    }
],
```

