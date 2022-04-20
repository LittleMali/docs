# Qt调试

本文记录一些Qt调试的技巧。

## 插件加载
* qt插件加载日志  
`export QT_DEBUG_PLUGINS=1`

## WebEngine日志
* 远程调试，类似F12  
  1. 开启debug， `export QTWEBENGINE_REMOTE_DEBUGGING=9988`
  2. 本地浏览器打开 localhost:9988
* QtWebEngineProcess日志  
  设置qt webEngine内核标记。
  1. --enable-logging：开启日志。
  2. --disalbelogging：关闭日志。
  3. --log-level=0/1/2/3：日志等级，从低到高，0是全部打印。
  4. --single-process: 单进程模式。
 `export QTWEBENGINE_CHROMIUM_FLAGS="--enable-logging --log-level=0 --disable-gpu"`
 
 