# QtWebEngine日志
QtWebEngine使用的是Chrome内核，很多设置跟chrome是类似的。通过命令行可以控制QtWebEngine日志开关。

## 日志开关设置
1. --enable-logging：开启日志。
2. --disalbelogging：关闭日志。
3. --log-level=0/1/2/3：日志等级，从低到高，0是全部打印。
4. --single-process: 单进程模式。
5. --enable-logging：还可以设置日志输出位置。

* 开启日志。
`export QTWEBENGINE_CHROMIUM_FLAGS="--enable-logging --log-level=0 --disable-gpu"`
* 开启日志并将日志输出到error流。
`export QTWEBENGINE_CHROMIUM_FLAGS="--enable-logging=stderr --log-level=0 --disable-gpu"`
* 开启日志并将日志输出到文件~/webengine.log。
`export QTWEBENGINE_CHROMIUM_FLAGS="--enable-logging=~/webengine.log --log-level=0 --disable-gpu"`

## QtWeb命令行
QtWebEngine 的浏览器的实际工作进程是 QtWebEngineProcess ，我们写一个内嵌页的ui程序最终会拉起 QtWebEngineProcess 来做实际的页面加载功能，因此，输出实际是 QtWebEngineProcess 的能力。所以，我们要把命令行参数传给 QtWebEngineProcess 。

具体参见[QtWebEngine命令行](./QtWeb%E4%BC%A0%E5%91%BD%E4%BB%A4%E8%A1%8C%E5%8F%82%E6%95%B0.md)