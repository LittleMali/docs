# openssl编译

## 源码

## 使用
### win端使用openssl
openssl不同版本编译出来的文件名不一样，在工程中使用的时候可以尝试搜索下面的名字。

openssl的lib名字：libeay32.lib、ssleay32.lib、libssl.lib、libcrypto.lib。
openssl的dll名字：libcrypto-1_1.dll、libssl-1_1.dll。


## 编译
* 单独编译openssl的步骤，没有看过。之前在编译libcurl的时候有涉及openssl，在win环境下，curl有提供脚本build-openssl.bat可以编译openssl。图简单省事的话，可以拉一份curl源码，在curl里面用bat编译openssl。