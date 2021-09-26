# TLS版本不匹配
这个问题并不是我跟的，只是记录一下，如何排查网络问题，对以后会有启发。

## 问题描述
  win客户端长链接https连接服务器失败。
## 分析过程
  1. 客户端长链接https是裸socket封装，tls握手使用的是openssl库。
  2. 客户端连接其他后台都是正常的，维护某个客户的服务器有问题。
  3. 排除win客户端代码修改，历史无提交修改记录。
  4. 客户端日志显示，https请求失败，应答超时，此日志无助于分析问题。
  5. 只好抓包了，wireshark抓包分析，过滤服务器ip:port，长链接请求一时不好触发，那就逐个tcp建联去看吧。
  6. tcp建联，syn -- syn ack -- ack，tcp握手成功，随后，客户端push data到后台，后台应答ack，再之后，客户端发起了fin包。
  7. 很诧异客户端为何会主动发fin包，再仔细看，客户端是5s以后主动发起了fin，好，查看代码，客户端确实有设置5000ms超时。
  8. 客户端主动断开tcp的原因找到了，但是，为什么5s之内都没有数据交互呢？tcp握手之后应该是tls的握手协议，openssl那一套函数要开始工作才对。wireshark的分析再看不到有用的信息了。
  9. 换个思路，客户端长链接会失败，那换个工具呢，拿curl直接去连服务器。
  10. 在同事的mac上，curl配置参数，连接服务器成功。
  11. 在自己的win上，curl配置参数，连接服务器失败。
  12. 打开curl回显，发现“SNI is not supported by OS"
   ![20210924093408](https://raw.githubusercontent.com/LittleMali/docs/master/mdPics/20210924093408.png)
  13. 拿wireshark抓curl发包的过程，发现tls握手的client hello并没有成功。
   ![20210924093524](https://raw.githubusercontent.com/LittleMali/docs/master/mdPics/20210924093524.png)
  14. 为什么tls握手失败了，提示的sni不支持？因为我们的长链接https是ip直连的，是以https:ip:port的格式去访问的。既然不支持，那就是tls协议版本的问题了。
  15. tls 1.3版本才支持ip：port格式的请求，但是，对于客户端来说，win10默认才支持到tls 1.2。
   ![20210924093805](https://raw.githubusercontent.com/LittleMali/docs/master/mdPics/20210924093805.png)
  16. 好了，问题初步查到了，用户服务器莫名的使用了tls 1.3的版本，但是客户端只是tls 1.2。
  17. 再回溯一下，为什么别的客户服务器是正常的呢？我们的后台程序使用的是go开发，go标准库可能使用了系统ssl库。
## 问题继续
  客户端确认了，不是tls的问题，客户说他们就没设置过tls版本，不承认我们定位的原因。
## 分析继续
  1. 写一个tls协议测试工具，继续分析。
  2. wireshark抓包，发现下面这个包。
   ![20210924094526](https://raw.githubusercontent.com/LittleMali/docs/master/mdPics/20210924094526.png)
   ![20210924094554](https://raw.githubusercontent.com/LittleMali/docs/master/mdPics/20210924094554.png)
   3. squid是用来做http反向代理了，这个代理超时了，squid 2.7又是2010年发布的版本，那么，是否是因为squid在中间挡了一道，导致tls握手失败？因此，需要客户去确认一下，squid的配置是否能支持tcp层了，因为我们的客户端实际是tcp来模拟了https协议。