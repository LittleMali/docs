# Docker

## 安装
官方文档： https://docs.docker.com/engine/install/ubuntu/

### 使用仓库安装
1. 卸载旧版本docker。
sudo apt-get remove docker docker-engine docker.io containerd runc

2. 安装依赖
更新apt-get，安装包使得apt可以使用https。
 sudo apt-get update

 sudo apt-get install \
    ca-certificates \
    curl \
    gnupg \
    lsb-release

3. 添加Docker官方的GPG密钥
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg

4. 设置stable仓库
echo \
  "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] https://download.docker.com/linux/ubuntu \
  $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null

5. 安装docker引擎
sudo apt-get update
sudo apt-get install docker-ce docker-ce-cli containerd.io

若要安装低版本的可以选择版本号。
查看版本list。
apt-cache madison docker-ce

选择一个版本安装。
sudo apt-get install docker-ce=<VERSION_STRING> docker-ce-cli=<VERSION_STRING> containerd.io

6. 测试docker
启动Docker服务。
sudo service docker start

下载测试映像并在容器中运行它。当容器运行时，它会打印一条消息并退出。
sudo docker run hello-world

7. 卸载Docker
#卸载Docker Engine、CLI和Containerd包
sudo apt-get purge docker-ce docker-ce-cli containerd.io
#主机上的映像、容器、卷或自定义配置文件不会自动删除。删除所有镜像、容器和卷
sudo rm -rf /var/lib/docker
sudo rm -rf /var/lib/containerd
