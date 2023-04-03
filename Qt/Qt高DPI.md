# Qt高dpi

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