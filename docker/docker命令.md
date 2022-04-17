# docker命令

* 查看docker镜像  
  `docker imgages`

* 查看docker容器  
  `docker ps`

* 启动/停止docker  
  `docker start <container_name/container_id>`  
  `docker stop <container_name/container_id>`  
  `docker restart <container_name/container_id>`

* 进入docker  
  `docker attach <容器名称>`  
  `docker exec -it <容器名称> /bin/bash`

* docker后台运行  
  `docker run -itd --name <>`

* 退出容器  
  `Ctrl+P`

* 导入/导出容器  
  `docker export <容器id> ubuntu.tar`  
  `cat ubuntu.tar | docker import - test/ubuntu:v1`

* docker删除  
  `docker rm -f <container_id>`