# openssl命令

* 查看版本
`openssl version -a`

* 查看支持的加密算法
`openssl ciphers -V`

* 查看openssl支持的tls/ssl协议版本  
  * `openssl s_client -help 2>&1 | awk '/-(ssl|tls)[0-9]/{print $1}'`
  * `openssl ciphers -ssl3`  

```
# 方法1：看help里面列出来的设置，可以知道支持哪些版本。
>openssl s_client -help
 -tls1                      Just use TLSv1
 -tls1_1                    Just use TLSv1.1
 -tls1_2                    Just use TLSv1.2
 -tls1_3                    Just use TLSv1.3
 -tsmssl                    Just use tsmssl
```
```
# 方法2：看密钥组件详情，如果支持某个版本，就会打印对应的算法，否则就是不支持。
>openssl.exe ciphers -ssl3
ciphers: Option unknown option -ssl3
ciphers: Use -help for summary.

>openssl.exe ciphers -tls1_1
TLS_AES_256_GCM_SHA384:TLS_AES_128_GCM_SHA256:TLS_CHACHA20_POLY1305_SHA256:T
```