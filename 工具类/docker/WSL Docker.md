# WSL Docker

win linux子系统，安装docker以后会run失败问题。
* 问题
  docker安装以后，运行测试例子，sudo docker run hello-world 会失败。
  错误描述： ERROR: for container_name  Cannot start service container_name: OCI runtime create failed: container_linux.go:349: starting container process caused "process_linux.go:319: getting the final child's pid from pipe caused \"EOF\"": unknown

* 解决方案： 升级WSL版本。
  问题文档： https://stackoverflow.com/questions/63072112/cannot-start-container-oci-runtime-create-failed-container-linux-go349
  1. win cmd查看wls版本
   wsl -l -v
   低版本会输出1，这里要升级为2 。
  2. 安装wls 2补丁。
   见微软文档： https://docs.microsoft.com/zh-cn/windows/wsl/install-manual#step-3---enable-virtual-machine-feature

   * 要先启用虚拟化。
   ![20220402175458](https://raw.githubusercontent.com/LittleMali/docs/master/mdPics/20220402175458.png)
   * 下载微软连接里面的补丁并安装。
   * 设置为新版本。
  wsl --set-default-version 2
   * 卸载&&重新安装docker。