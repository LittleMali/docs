# QWebEngineUrlScheme

https://doc.qt.io/qt-5/qwebengineurlschemehandler.html

Note: Make sure that you create and register the scheme object before the QGuiApplication or QApplication object is instantiated.

```c++
// Qt5.15
class MySchemeHandler : public QWebEngineUrlSchemeHandler
{
public:
    MySchemeHandler(QObject *parent = nullptr);
    void requestStarted(QWebEngineUrlRequestJob *request)
    {
        // ....
    }
};

int main(int argc, char **argv)
{
    QWebEngineUrlScheme scheme("myscheme");
    scheme.setSyntax(QWebEngineUrlScheme::Syntax::HostAndPort);
    scheme.setDefaultPort(2345);
    scheme.setFlags(QWebEngineUrlScheme::SecureScheme);
    QWebEngineUrlScheme::registerScheme(scheme);

    // ...
    QApplication app(argc, argv);
    // ...

    // installUrlSchemeHandler does not take ownership of the handler.
    MySchemeHandler *handler = new MySchemeHandler(parent);
    QWebEngineProfile::defaultProfile()->installUrlSchemeHandler("myscheme", handler);
}
```