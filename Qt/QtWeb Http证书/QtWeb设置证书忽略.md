# QtWebEngine证书设置

有些时候，某些服务器是自签证书，自签证书无法通过浏览器校验，我们可以设置忽略服务器证书校验。

```c++
WebPage::WebPage(QWidget *parent) {
    setGeometry(0, 0, parent->width(), parent->height());

    // QWebEngineView *m_webview = Q_NULLPTR; // defined in .h file

    m_webview = new QWebEngineView(this);
    m_webview->setPage(new WebPageUnSafe(this));  // 证书忽略
    m_webview->setFixedSize(parent->width(), parent->height() + 10);

    m_webview->page()->profile()->setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);
    m_webview->page()->profile()->setHttpCacheType(QWebEngineProfile::MemoryHttpCache);
}
```
```c++
#include <QWebEngineCertificateError>
#include <QWebEnginePage>
// 不校验服务器证书
class WebPageUnSafe : public QWebEnginePage {
    Q_OBJECT
public:
    explicit WebPageUnSafe(QWidget* parent = 0) : QWebEnginePage(parent) {}

protected:
    bool certificateError(const QWebEngineCertificateError& error) override { return true; }
};
```
