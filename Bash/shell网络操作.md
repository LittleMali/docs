# shell命令

## 网络相关
### 端口状态
```shell
-p: 协议。
-a：all，所有连接和监听的端口。
-b：连接所涉及的进程。

netstat -p tcp -a -b

netstat -ano | findstr 8080
```

### 路由表  
`netstat -r`  
`route -n`

### 域名解析
``` shell
C:\Users\Administrator>nslookup www.taobao.com
服务器:  gm-heiji.tencent.com
Address:  10.11.56.22

非权威应答:
名称:    www.taobao.com.danuoyi.tbcache.com
Addresses:  240e:ff:d800:500:3::3fa
          240e:ff:d800:500:3::3f9
          14.215.57.233
          14.215.57.232
Aliases:  www.taobao.com
```
使用type参数来指定解析的ip类型。  
A Record：Address record，是一组32位ipv4地址。  
AAAA Record：ipv6 address record，是一组128位ipv6地址。  
```shell
C:\Users\Administrator>nslookup -type=A www.taobao.com
服务器:  gm-heiji.tencent.com
Address:  10.11.56.22

非权威应答:
名称:    www.taobao.com.danuoyi.tbcache.com
Addresses:  14.215.57.232
          14.215.57.233
Aliases:  www.taobao.com

```
```shell
C:\Users\Administrator>nslookup -type=AAAA www.taobao.com
服务器:  gm-heiji.tencent.com
Address:  10.11.56.22

非权威应答:
名称:    www.taobao.com.danuoyi.tbcache.com
Addresses:  240e:ff:d800:500:3::3fa
          240e:ff:d800:500:3::3f9
Aliases:  www.taobao.com
```

### host文件
Linux下本地域名解析文件`/etc/resolv.conf`


### 代理设置
```
# su - username -c "export DISPLAY=:0 && gsettings set org.gnome.system.proxy mode 'manual' && gsettings set org.gnome.system.proxy.http host '127.0.0.1' && gsettings set org.gnome.system.proxy.http port 12345  && gsettings set org.gnome.system.proxy.https host '127.0.0.1' && gsettings set org.gnome.system.proxy.https port 12345"

```