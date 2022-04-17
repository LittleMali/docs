# curl

## 源码
https://github.com/curl/curl.git

### win编译
/curl/projects/下有各个版本的visual studio可以使用。
```bash
# 生成vs 2005工程
generate.bat vc8
```

## curl命令
使用 --ipv4 或者 -4 强制使用ipv4协议。
使用 --ipv6 或者 -6 强制使用ipv6协议。

## ipv4和ipv6选择
cURL 对ipv4、ipv6是这样一个处理过程：
1. 解析被请求的域名，通常会获得一个ip地址列表，同时包含了ipv4和ipv6地址;
2. cURL会首先发起连接ipv6地址，如果200ms内连接成功，则直接使用此地址；
3. 如果200ms未连接成功ipv6地址，cURL会对列表中地址并发发起连接；
4. 最后等待第一个成功建立的连接，将作为后续cURL传输地址。  
从上面流程可以看出几点：ipv6优先机制、快乐眼球（Happy Eyeballs）机制（也叫快速回退Fast Fallback机制）、轮询调度机制（Round-robin DNS）。
### Happy Eyeballs
“快乐眼球”是客户端的一个算法，它叫这么个名字，是想表达这个算法是用在互联网中的用户端（客户端）而不是服务器端。  
cURL的实现不是简单的执行：getaddrinfo() -> 遍历每个ip地址然后并发发起 connect()。它的做法将全部地址分开到一个ipv4专用和一个ipv6专用的两个线程队列，然后再并发发起连接，最先收到回复的连接作为最终使用的连接。  
这个算法的提出是在2011年 World IPv6 Day，为了避免 ipv6 起步阶段网络环境不理想。  
除了cURL，目前支持 Happy Eyeballs 其它软件包括 Chrome 浏览器, Opera 浏览器, Firefox 13+, 苹果操作系统 OS X 等。

### Round-robin DNS
这是DNS服务器的轮询调度算法，至于为啥叫Round-robin？这个词源于法语ruban rond（round ribbon），意思是环形丝带。  
轮询调度经常用在负载均衡的处理上，如为一个域名配置多个ip地址：
```
server  IN  A  192.168.0.1
server  IN  A  10.0.0.1
server  IN  A  127.0.0.1
```
当对域名的并发请求过来时，服务器会按顺序轮流返回ip地址。这个算法的好处是实现简单、机会公平均等。缺点也比较明显，就是不同时刻的请求可能会获得不同的地址，这会对客户端基于ip地址的连接共享、缓存等造成影响。如果一个服务器ip结点宕机了，它的ip地址可能还会被返回使用。

## Set Option选项
### Dns相关选项
* CURLOPT_IPRESOLVE  
  指定libcurl域名解析的方式，其值有：
  * CURL_IPRESOLVE_WHATEVER：默认值，相当于PF_UNSPEC，支持IPv4/v6，具体以哪个优先需要看libc底层实现，Android中默认以IPv6优先，当IPv6栈无法使用时，libcurl会用IPv4。
  * CURL_IPRESOLVE_V4：.仅请求A记录，即只解析为IPv4地址。
  * CURL_IPRESOLVE_V6：.仅请求AAAA记录，即只解析为IPv6地址。
  注意：该功能生效的前提是libcurl支持IPv6，需要在curl/lib/curl_config.h配置#define ENABLE_IPV6 1。

* CURLOPT_DNS_CACHE_TIMEOUT  
  设置libcurl DNS缓存超时时间，默认为60秒，即每60秒清一次libcurl自身保存的DNS缓存。  
  如果设置为0，则不使用DNS缓存，设置为-1，则永远不清缓存。
* CURLOPT_DNS_USE_GLOBAL_CACHE  
  让libcurl使用系统DNS缓存，默认情况下，libcurl使用本身的DNS缓存。
  