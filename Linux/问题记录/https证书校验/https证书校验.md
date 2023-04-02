# https证书校验

## QtWebEngine设置证书忽略
使用Chrome浏览器访问某个自签服务器资源时，Chrome会提示不安全的访问，我们忽略&强制访问，才会继续加载服务器资源。  
在QtWebEngine加载url时，需要显示设置忽略服务器自签证书。具体步骤参考*Qt内容*。  

## 服务配置问题导致QtWebEngine异常
我们遇到了一个场景，Chrome浏览器中访问某个资源时，首次提示证书告警，信任以后每次都可以正常加载资源。但是，在QtWebEngine中，我们无法打开页面。
1. 浏览器中加载资源，查看证书内容。
   证书是一个三级子证书，是合法的CA签发的证书。
2. 手动校验证书是否正确。
   在浏览器里面访问资源，把证书保存到本地，使用openssl校验证书是否正确。  
   这一步，证书正确。
3. 在浏览器中，证书呈现三级证书链。
   我们去Linux系统中找，根证书存在，中间证书不在。难道是这一步导致Qt证书校验失败了？  
4. Qt或者浏览器加载url时，https握手协商的时候，是服务器下发证书信息，浏览器校验证书是否合法。
   我们wireshark抓包看，Client Hello，Server Hello以后，服务器下发证书，我们看到了一个证书——三级子证书，服务器下发证书以后client就断开了连接。  
   我们wireshark继续抓包看一个正常的资源，比如baidu.com，服务器下发的是一个证书链——三级证书链。  
5. 服务器在下发证书时，如果下发的服务器证书无法单独验证证书合法性，那么，服务器应该下发一个完整的证书链。
   根CA -- 中间CA -- 子证书，这是一个证书签发的流程。反过来，子证书的校验需要回溯中间证书和根证书。因此，服务器要下发完整证书链。
6. 服务器ngnix配置证书链。
   参考文档[如何补全 SSL 证书链](https://cloud.tencent.com/document/product/400/52983)。
   cat 子证书 中间证书 根证书 > 证书链

## QtWebEngine5.12版本
服务器下发了三级证书链，但是QtWeb依旧无法打开页面。虽然我们设置了证书忽略，但是没生效。
看Qt源码，是某些场景的错误处理，没有走到证书忽略里面去。

我们继续用wireshark抓包，证书下发是一个完整的证书链，证书校验是ok的，https握手协商以后，到数据包的部分，收到一个包以后，客户端就发了Encrypted Alert，断开了连接。  
一个是思路是去看qt源码，尝试修改，这个代价太大。  
另一个思路是尝试分析一下Alert，看看到底报了什么错。

## wireshark分析https Encrypted Alert
参考文章[关于 Wireshark 分析 https 的 tcpdump 中的 Encrypted Alert](https://www.tianxiaohui.com/index.php/default/%E5%85%B3%E4%BA%8E-Wireshark-%E5%88%86%E6%9E%90-https-%E7%9A%84-tcpdump-%E4%B8%AD%E7%9A%84-Encrypted-Alert.html)  


因为https握手协商已经完成了，所以，数据包已经被加密传输了，所以，Encrypted Alert这个数据包也是加密的，我们没有办法直接看到alert的错误码。  
按照这篇文章，核心的步骤是要浏览器自动生成的ssl key log —— https握手协商时约定出来的Pre Master，后续wireshark导入ssl key log文件，就可以解密数据了。

1. 设置环境变量 `export SSLKEYLOGFILE=~/ssh_key.log` 。
2. 打开Chrome浏览器，确认`~/ssh_key.log`日志是否生成。
   大概率是看不到log文件的，可以参考下面的步骤设置一下。
3. 浏览器支持https协议，要看其使用的ssl协议，比如openssl。
   `openssl s_client -connect example.com:443 -tls1_3 -keylog /path/to/keylog/file`  
   使用openssl命令来验证，浏览器使用的ssl协议层是否支持这个功能。  
   -keylog参数是在OpenSSL 1.1.1版本中引入的，用于输出TLS会话密钥到指定的日志文件中，以便在调试和分析TLS会话时使用。
4. 设置wireshark。
   菜单 -> 选项 -> 协议 -> TLS -> (Pre)-Master-Secret log filename 为我们上面的key文件。  
   之后，wireshark抓包就能看到解密以后的数据了。


### chrome生成日志
如果设置了 `SSLKEYLOGFILE` 但是没有生成日志，可能是Chrome没有开启这个功能。
1. 确认Chrome浏览器是否支持TLS 1.3协议。在地址栏中输入chrome://flags，然后查找“TLS 1.3”选项并启用它。
2. 确认Chrome浏览器是否已启用TLS密钥日志记录功能。在地址栏中输入chrome://version，然后查找“Command Line”一栏中是否包含`--ssl-key-log-file`参数。如果没有，请将其添加到启动参数中。
3. 重新启动Chrome浏览器，并访问一个使用TLS 1.3协议的网站，检查 `SSLKEYLOGFILE` 环境变量是否生成了相应的文件。

在Chrome的设置选项中，没有看到Tls 1.3的设置，更重要的是命令行里面的`--ssl-key-log-file=~/ssh_key.log`参数。  
一个取巧的方式可以在Chrome追加命令行：修改chrome的快捷方式，在命令行里面追加一项。

### chrome设置命令行
在Chrome浏览器中添加命令行参数的方法如下：
1. 打开Chrome浏览器。
2. 在地址栏中输入chrome://version/并按下回车键。
3. 查找“命令行”部分，这里会显示Chrome浏览器当前使用的命令行参数。
4. 在同一行的最后添加命令行参数，例如--disable-extensions。
5. 关闭Chrome浏览器并重新启动，新的命令行参数将生效。

说明，在win下这个方法不行，`chrome://version/`显示的命令行选项是没法编辑修改的。