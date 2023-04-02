# QtWebEngine传命令行参数

QtWebEngine会将参数透传给QtWebEngineProcess。
```c++
int main(int argc, char* argv[]) {
    QCoreApplication::setOrganizationName("QtExamples");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv); // 参数透传

    MainWindow w;
    w.show();

    return app.exec();
}
```

最核心的代码是 `QApplication app(argc, argv)` ，如果我们的应用程序没有办法直接拿到main函数的参数，那么，我们可以自己定义类似argc和argv的两个参数，传入QApplication中。

## Note
注意，上面的一行代码`QCoreApplication::setOrganizationName("QtExamples")`，这一行要加上，某个场景下有坑，记不得了。