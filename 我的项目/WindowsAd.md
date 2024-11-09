# Windows AD

## Active Directory
Active Directory中文翻译为活动目录，这个概念不需要太过深入纠结，简单的理解它：Active Directory（活动目录）是微软Windows Server中，负责架构中大型网路环境的集中式目录管理服务（Directory Services），Windows 2000 Server开始内建于Windows Server产品中，它处理了在组织中的网路物件，物件可以是计算机，用户，群组，组织单元（OU）等等，只要是在Active Directory结构定义档（schema）中定义的物件，就可以储存在Active Directory资料档中，并利用Active Directory Service Interface来存取。

## Actiove Directory作用
AD(Actiove Directory简称)微软构建这样的一个服务它的意义何在？个人认为可以分为以下几个方面来了解：

1. 用户服务
管理用户的域账号、用户信息、企业通信录（与电子邮箱系统集成）、用户组管理、用户身份认证、用户授权管理、按需实施组管理策略等。这里不单单指某些线上的应用更多的是指真实的计算机，服务器等。C#语言能涉及的领域非常之广泛，根据Windows提供的.NET Framework和PowerShell完全可以做到监听某机器的按键或获取最高权限等操作。
2. 计算机管理
管理服务器及客户端计算机账户、所有服务器及客户端计算机加入域管理并按需实施组策略。
3. 资源管理
管理打印机、文件共享服务、网络资源等实施组策略。
4. 应用系统的支持
对于电子邮件（Exchange）、在线及时通讯（Lync）、企业信息管理（SharePoint）、微软CRM,ERP等业务系统提供数据认证（身份认证、数据集成、组织规则等）。这里不单是微软产品的集成，其它的业务系统根据公用接口的方式一样可以嵌入进来。
5. 客户端桌面管理
系统管理员可以集中的配置各种桌面配置策略，如：用户适用域中资源权限限制、界面功能的限制、应用程序执行特征的限制、网络连接限制、安全配置限制等。

## 总述
AD是微软基于LDAP在Windows上的实现，是LDAP认证协议的具体实现。