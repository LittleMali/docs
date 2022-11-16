# Qt调试

本文记录一些Qt调试的技巧。

## 插件加载
* qt插件加载日志  
`export QT_DEBUG_PLUGINS=1`

## WebEngine日志
* QtWebEngineProcess日志  
  设置qt webEngine内核标记。
  1. --enable-logging：开启日志。
  2. --disalbelogging：关闭日志。
  3. --log-level=0/1/2/3：日志等级，从低到高，0是全部打印。
  4. --single-process: 单进程模式。
 `export QTWEBENGINE_CHROMIUM_FLAGS="--enable-logging --log-level=0 --disable-gpu"`
 
 ## WebEngine调试
 * 远程调试，类似F12  
  1. 开启debug， `export QTWEBENGINE_REMOTE_DEBUGGING=9988`
  2. 本地浏览器打开 localhost:9988
  3. 高版本的chrome不支持远程调试，无法加载devtools。要使用低版本的chrome才行。
  https://qa.1r1g.com/sf/ask/4220449201/
  https://developer.tizen.org/forums/web-application-development/tizen-studio-debug-tizen-web-application-blank-page-chrome-80

Windows: https://commondatastorage.googleapis.com/chromium-browser-snapshots/index.html?prefix=Win_x64/706915/
Mac: https://commondatastorage.googleapis.com/chromium-browser-snapshots/index.html?prefix=Mac/706915/
Linux: https://commondatastorage.googleapis.com/chromium-browser-snapshots/index.html?prefix=Linux_x64/706915/