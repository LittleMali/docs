# PAC代理
PAC（Proxy Auto-Config）代理是一种自动代理配置的方法，用于根据特定规则自动选择代理服务器。PAC 文件是一个包含一系列 JavaScript 函数的文本文件，用于定义代理规则。

PAC 代理的工作原理是在每次发起网络请求时，浏览器或操作系统会根据 PAC 文件中的规则来判断是否需要通过代理服务器进行访问。PAC 文件中的规则可以根据 URL、主机名、IP 地址等来匹配请求，以决定是否使用代理。

PAC 文件中的规则可以非常灵活，你可以根据需要定义不同的规则。例如，你可以指定某些网址或域名使用代理，而其他网址直接连接，或者根据时间、地理位置等条件来选择代理服务器。

PAC 代理的优点是可以根据具体需求灵活配置代理规则，使得不同的网络流量可以通过不同的代理服务器进行访问。这对于需要访问特定网站或需要绕过某些网络限制的情况非常有用。

PAC 代理通常用于企业网络环境中，以便根据公司的网络策略和需求来自动选择代理服务器。它也可以用于个人用户，以便根据个人需求来配置代理规则。

## Ubuntu PAC代理设置
```
gsettings set org.gnome.system.proxy mode 'manual'
gsettings set org.gnome.system.proxy.http host '127.0.0.1' 
gsettings set org.gnome.system.proxy.http port 8080 
gsettings set org.gnome.system.proxy.https host '127.0.0.1'
gsettings set org.gnome.system.proxy.https port 8080
```

1. user态下设置gsettings，不要加sudo。
2. 系统设置里面会有一个代理设置的界面，这个系统界面不一定会跟着显示开启。
3. 在kylin系统下这么设置，是可以生效的。
4. 我们需要自己写一个代理服务器，listen 8080端口，处理流量。

