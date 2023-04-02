# QtWeb F12调试

## WebEngine外置浏览器调试
开启远程调试，类似浏览器的F12功能。  
1. 开启debug本地端口， `export QTWEBENGINE_REMOTE_DEBUGGING=9988`。
2. 本地浏览器打开 localhost:9988。在localhost中可以看到QtWebEngine加载的页面。

**注意**
开启远程调试以后，如果在浏览器（Chome等）中提示以下错误，则是浏览器没有开启或者不支持远程调试了。
```js
Uncaught TypeError: document.registerElement is not a function
    at Object.UI.registerCustomElement (shell.js:formatted:18064)
    at shell.js:formatted:18178
    at shell.js:formatted:18266
devtools_app.js:formatted:2606 Uncaught ReferenceError: SDK is not defined
    at devtools_app.js:formatted:2606
```
按照网上的说法，是Chrome 80已弃用这些开发工具使用的WebComponents v0。  
解决方案是使用额外的参数启动 Chrome 以重新启用该功能（当它仍在引擎中时）。  
`--enable-blink-features=ShadowDOMV0,CustomElementsV0`

参考文档：[Chrome 开发人员工具不再有效](https://qa.1r1g.com/sf/ask/4220449201/)

## WebEngine代码实现F12调试
开启外置的浏览器F12调试工具，可能有chrome版本不支持的问题，那么，我们可以提前在代码中内置F12调试工具。

```c++
WebPage::WebPage(QWidget *parent) {
    setGeometry(0, 0, parent->width(), parent->height());

    // QWebEngineView *m_webview = Q_NULLPTR; // defined in .h file

    m_webview = new QWebEngineView(this);
    m_webview->setPage(new WebPageUnSafe(this));
    m_webview->setFixedSize(parent->width(), parent->height() + 10);

    if (DebugCfg()->IsWebF12()) {
        // 上半部分为正常web
        // 下半部分为调试器
        QSplitter *pSplitter = new QSplitter(this);
        pSplitter->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        pSplitter->setOrientation(Qt::Vertical);
        pSplitter->setHandleWidth(0);

        auto pInspectorView = new QWebEngineView(this);
        pInspectorView->setFixedSize(parent->width(), parent->height() / 2);

        m_webview->page()->setDevToolsPage(pInspectorView->page());  // 插入F12工具
        m_webview->setFixedSize(parent->width(), parent->height() / 2);

        pSplitter->addWidget(m_webview);
        pSplitter->addWidget(pInspectorView);

        QVBoxLayout *pMainLayout = new QVBoxLayout(this);
        pMainLayout->addWidget(pSplitter);
    }

    m_webview->page()->profile()->setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);
    m_webview->page()->profile()->setHttpCacheType(QWebEngineProfile::MemoryHttpCache);
}
```