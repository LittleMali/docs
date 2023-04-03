# Qt插件路径
基于Qt开发的ui进程，在运行时需要加载qt库，比如libQt5Core.so。在默认情况下，Qt会去系统路径下寻找Qt库。那么这会引起几个问题。
* 一个新的系统要提前安装qt才能使用。
* 系统中已经有了qt但是qt的版本不一致，导致ui进程无法运行。
因此，在实际发布我们ui文件时，我们应该自己携带一套所依赖的qt库，让ui进程去我们自己的安装路径下加载qt插件。类似于win下带一套运行时库。

## qtconf
在ui进程所在的路径下放置qt.conf文件，指明qt库的路径所在。
* `Libraries= ./plugins/`
  qt库所在的路径，主要是libQt5Core等so文件，也包括iconengines，imageformats等文件夹。这些文件夹从安装以后的qt中拷贝一份即可。
* `Translations= ./translations/`
  因为我们使用到了QtWebEngine，所以，其依赖也要一并打包。translations，resources，plugins/platforms等文件夹也是需要的。

```bash
# 安装根目录下，查看文件结构
$ tree -d 
.
├── bin
│   ├── ui
│   ├── qt.conf
│   ├── ...
│   └── ...
├── plugins
│   ├── libQt5Core.so.5
│   ├── libQt5WebEngineCore.so.5
│   ├── libQt5Widgets.so.5
│   ├── ...
│   ├── iconengines
│   ├── imageformats
│   ├── platforms
│   └── position
│       └── libqxcb.so
├── resources
├── translations
│   └── qtwebengine_locales
└── ...

# ui同一级目录下，看看qt.conf文件内容
$ cat qt.conf 
[Paths]
Prefix= ../
Libraries= ./plugins/
Plugins= ./plugins/
Imports= ./qml/
Translations= ./translations/
Qml2Imports= ./qml/

# Prefix表示ui上跳一级以后去寻找。
```

虽然我们配置了qt.conf文件，但是，某些第三方软件可能会修改QT环境变量，导致ui进程去读取环境变量，从而忽略了qt.conf文件。  
下面的代码查看了qt环境变量，这里是一个*正确*的环境变量，里面没有`QT_PLUGIN_PATH`设置，`QT_PLUGIN_PATH`是配置qt插件路径，告诉qt去哪里寻找so库依赖。  

**`QT_PLUGIN_PATH`的优先级高于qt.conf文件**

```bash
$ export | grep QT
declare -x QT4_IM_MODULE="fcitx"
declare -x QTWEBENGINE_CHROMIUM_FLAGS="--enable-logging=~/webengine.log --log-level=0"
declare -x QT_DBL_CLICK_DIST="30"
declare -x QT_DBL_TAP_DIST="30"
declare -x QT_IM_MODULE="fcitx"
declare -x QT_QPA_PLATFORM="xcb"
declare -x QT_QPA_PLATFORMTHEME="ukui"
declare -x QT_SCALE_FACTOR_ROUNDING_POLICY="PassThrough"
```
## qt库
保险起见，我们在代码中设置qt库的依赖路径。
```c++
int main(int argc, char const* argv[],) {
    // 添加自定义的plugin路径，避免外部环境中的 QT_PLUGIN_PATH 的影响
    // 环境变量中的 QT_PLUGIN_PATH 的优先级比 qt.conf 更高
    QString qpath = "install_dir/plugins";
    QCoreApplication::addLibraryPath(qpath);

    // 此行属性设置必须置于 QApplication 构造之上
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(_argc, _argv);
    QCoreApplication::setOrganizationName("Little");
}
```