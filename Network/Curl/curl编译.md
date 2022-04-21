# curl编译

源码
https://github.com/curl/curl.git

## 编译
1. git 拉取代码。
2. 切换到某个tag。

### win编译- no openssl
  win环境下，使用Visual Studio编译比较方便。
1. 在 curl\projects\目录下，README文档记录了编译方法。
2. 在 curl\projects\目录下，generate.bat 配置使用哪个版本的vs。
3. 例如，进入 curl\projects\Windows\VC10 ，打开 curl-all.sln ，编译curl和libcurl。

编译选项：
DLL：生成libcurl.dll，第三方程序可以集成dll使用。  
LIB：生成静态libcurl.lib，第三方程序静态链接lib使用。

验证curl.exe，我们可以看到编译出来的curl是不支持https协议的。
```
curl\build\Win32\VC8\LIB Release>curl.exe --version
curl 7.79.1 (i386-pc-win32) libcurl/7.79.1
Release-Date: 2021-09-22
Protocols: dict file ftp gopher http imap ldap ldaps mqtt pop3 rtsp smb smtp telnet tftp
Features: AsynchDNS HSTS Largefile NTLM UnixSockets alt-svc
```

### win编译 - with openssl
  win环境下，使用Visual Studio编译比较方便。
1. 在 curl\projects\目录下，README文档记录了编译方法。
2. 准备openssl git代码。如下是目录结构。
```
   somedirectory\
    |_curl
    | |_ build
    |    |_<architecture>
    |      |_<ide>
    |        |_<configuration>
    |          |_lib
    |          |_src
    | |_ projects
    |     |_ build-openssl.bat
    |
    |_openssl
    | |_ build
    |    |_<architecture>
    |      |_VC <version>
    |        |_<configuration>
```
3. 在 curl\projects\目录下， build-openssl.bat 脚本会先编译 openssl。  
这个脚本在内部会定位openssl的源码，在bat目录下，上跳两级寻找openssl目录。
4. build-openssl.bat 依赖 Perl，windows要先安装Perl。
5. 调用 build-openssl.bat 编译openssl，耗时大约十来分钟。  
`build-openssl.bat vc8 x86 release`
![20220421171009](https://raw.githubusercontent.com/LittleMali/docs/master/mdPics/20220421171009.png)
6. 在 openssl\build\Win64\VC8\LIB Release 下可以看到生成了openssl.exe、libcrypto.lib、libssl.lib和include文件。
7. 回到 curl\projects\目录下，generate.bat 配置生成vs环境。
8. vs选择 编译选项 LIB Release - LIB OpenSSL ，意思是静态链接 release版本的openssl lib，生成 静态libcurl.lib，同时也会生成curl.exe。此curl.exe是静态链接了 openssl 生成的lib。  
我们可以看到 curl 支持了https协议。
```
\curl\build\Win32\VC8\LIB Release - LIB OpenSSL>curl.exe --version
curl 7.79.1 (i386-pc-win32) libcurl/7.79.1 OpenSSL/1.1.1l
Release-Date: 2021-09-22
Protocols: dict file ftp ftps gopher gophers http https imap imaps ldap ldaps mqtt pop3 pop3s rtsp smb smbs smtp smtps telnet tftp
Features: AsynchDNS HSTS HTTPS-proxy Largefile NTLM SSL UnixSockets alt-svc
```
9. 使用 `curl.exe -v https://www.baidu.com`访问https页面，会报错：缺少证书。