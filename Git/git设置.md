# git设置

## 免密拉取
先设置个人信息  
git config --global user.email "you@example.com"  
git config --global user.name "Your Name"  

再保存  
git config --global credential.helper store

user.name=xx
user.email=xx
user.password=xxx
credential.helper=store

## 修改了git密码以后
1. 确认git使用什么机制缓存了凭据。
```
$ git config --global --get credential.helper 
store
```
2. 如果你使用的是store凭据助手，你的凭据将被存储在一个明文文件中（默认位置是~/.git-credentials）。要删除缓存的密码，你可以直接编辑这个文件，删除相应的行，或者使用命令行：
`git credential-store --file ~/.git-credentials erase host=example.com protocol=https`
3. 如果你使用的是cache凭据助手，你可以通过等待缓存过期来删除密码。默认情况下，缓存会在15分钟后过期。如果你想立即删除缓存的密码，你可以重启你的计算机或者杀死git-credential-cache--daemon进程