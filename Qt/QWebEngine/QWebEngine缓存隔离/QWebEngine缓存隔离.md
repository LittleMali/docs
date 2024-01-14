# WebEngine缓存隔离
QWebEngineProfile的构造函数接受一个可选的字符串参数，这个参数用于设置配置文件的名称。这个名称在磁盘上用于标识存储配置文件的目录。

当你创建一个新的QWebEngineProfile实例并为其提供一个名称时，Qt WebEngine会在用户的数据目录下创建一个与该名称相对应的子目录。这个子目录用于存储与该配置文件相关的所有数据，包括缓存、cookie、本地存储、插件数据等。

例如，以下代码：

```c++
// set cache dir
QWebEngineProfile *profile = new QWebEngineProfile("MyCache");
profile->setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);
profile->setHttpCacheType(QWebEngineProfile::MemoryHttpCache);

m_webview = new QWebEngineView(this);
m_webview->setPage(new WebPageUnSafe(profile, this));

auto webchannel = new QWebChannel;
webchannel->registerObject(QStringLiteral("qbridge"), this);
m_webview->page()->setWebChannel(webchannel);

// delete cache
auto cachePath = m_webview->page()->profile()->cachePath();
m_webview->page()->profile()->cookieStore()->deleteAllCookies();

auto path = m_webview->page()->profile()->persistentStoragePath();
QDir storagePath = path;
storagePath.removeRecursively();
```

将会在用户的数据目录下创建一个名为"iam"的子目录，用于存储与这个配置文件相关的所有数据。

如果你为QWebEngineProfile的构造函数提供的名称为空字符串，或者你完全不提供名称，那么Qt WebEngine将会使用默认的配置文件，并且所有的数据都会被存储在用户的数据目录的根目录下。

需要注意的是，每个配置文件都有自己的cookie存储、缓存、本地存储等，因此不同的配置文件之间是完全隔离的。这使得你可以为不同的web内容创建不同的配置文件，以实现不同的存储和缓存策略。