# LDAP认证
LDAP：Lightweight Directory Access Protocol，轻量级目录访问协议。

我们在开始介绍之前先来看几个问题：
1. 我们日常的办公系统是不是有多个？
2. 每个系统之间是不是都有独立的账号密码？
3. 密码多了，有时候半天想不起来哪个密码对应哪个系统？
4. 每次新项目的开发，都需要重新开发和维护一套用户密码？
5. 维护多套系统的用户是不是非常头疼？

如今大家再也不用为上面的的问题头疼了，因为“LDAP统一认证服务”已经帮助大家解决这些问题了。

## 什么是LDAP？
在介绍什么是LDAP之前，我们先来复习一个东西：“什么是目录服务？”
1. 目录服务是一个特殊的数据库，用来保存描述性的、基于属性的详细信息，支持过滤功能。
2. 是动态的，灵活的，易扩展的。
   
如：人员组织管理，电话簿，地址簿。

了解完目录服务后，我们再来看看LDAP的介绍：  
LDAP（Light Directory Access Portocol），它是基于X.500标准的轻量级目录访问协议。  
目录是一个为查询、浏览和搜索而优化的数据库，它成树状结构组织数据，类似文件目录一样。  
目录数据库和关系数据库不同，它有优异的读性能，但写性能差，并且没有事务处理、回滚等复杂功能，不适于存储修改频繁的数据。所以目录天生是用来查询的，就好象它的名字一样。  
LDAP目录服务是由目录数据库和一套访问协议组成的系统。

LDAP（轻型目录访问协议）是一种软件协议 ，使任何人都可以在公共互联网或公司内网上查找网络中的组织，个人和其他资源（例如文件和设备）的数据 。LDAP 是目录访问协议（DAP）的“轻量级”版本，它是 X.500（ 网络中目录服务的标准 ）的一部分。

目录告诉用户某些内容在网络中的位置。在 TCP / IP 网络上，域名系统（DNS）是用于将域名与特定网络地址（网络上的唯一位置）相关联的目录系统。但是，用户可能不知道域名。LDAP 允许用户搜索个人，而无需知道他们的位置（尽管其他信息将对搜索有所帮助）。

为什么要使用LADP？  
LDAP是开放的Internet标准，支持跨平台的Internet协议，在业界中得到广泛认可的，并且市场上或者开源社区上的大多产品都加入了对LDAP的支持，因此对于这类系统，不需单独定制，只需要通过LDAP做简单的配置就可以与服务器做认证交互。“简单粗暴”，可以大大降低重复开发和对接的成本。

## LDAP目录树
LDAP本质是个树状结构的数据库。
1. 目录树：在一个目录服务系统中，整个目录信息集可以表示为一个目录信息树，树中的每个节点是一个条目。
2. 条目：每个条目就是一条记录，每个条目有自己的唯一可区别的名称（DN）。
3. 对象类：与某个实体类型对应的一组属性，对象类是可以继承的，这样父类的必须属性也会被继承下来。
4. 属性：描述条目的某个方面的信息，一个属性由一个属性类型和一个或多个属性值组成，属性有必须属性和非必须属性。

| 厂商       | 产品                       | 介绍                                                         |
| ---------- | -------------------------- | ------------------------------------------------------------ |
| SUN        | SUNONE Directory Server    | 基于文本数据库的存储，速度快。                               |
| IBM        | IBM Directory Server       | 基于DB2 的的数据库，速度一般。                               |
| Novell     | Novell Directory Server    | 基于文本数据库的存储，速度快, 不常用到。                     |
| Microsoft  | Microsoft Active Directory | 对大数据量处理速度一般，但维护容易，生态圈大，管理相对简单。 |
| Opensource | Opensource                 | OpenLDAP 开源的项目，速度很快，但是非主 流应用。             |

## LDAP的用途
LDAP的常见用途是为身份验证提供中心位置 -- 意味着它存储用户名和密码。然后，可以将 LDAP 用于不同的应用程序或服务中，以通过插件验证用户。例如，LDAP 可用于 Docker，Jenkins，Kubernetes，Open VPN 和 Linux Samba 服务器验证用户名和密码。系统管理员还可以使用 LDAP 单一登录来控制对LDAP数据库的访问。

### Active Directory
Active Directory 是一种目录服务，用于管理域，用户和分布式资源（例如 Windows 操作系统的对象）。**目录服务背后的意义是它在管理域和对象的同时控制哪些用户可以访问每个资源**。Active Directory 在 Windows Server 10 上可用，并且由多种服务组成。Active Directory 中包括的服务包括域，轻型目录，证书，联合身份和权限管理服务。每个服务都包含在 Active Directory 名称下，以扩展目录管理功能。

Active Directory 包含有关整个网络上每个用户帐户的信息。它将每个用户帐户视为一个对象。每个用户对象还具有多个属性。属性的示例是用户的名字，姓氏或电子邮件地址。所有这些信息都存在于域控制器（Active Directory）上的大型加密数据库中。挑战在于以可用格式提取信息。这是 LDAP 的主要工作。

## 总述
* LDAP是一个网络协议，相对于DAP而言的轻量级的协议。约定了 Client 与 Server 之间的信息交互格式、使用的端口号、认证方式等内容。
* LDAP的实现（例如Windows AD）是一个树状的数据库（叫 用户目录），树状的数据库天然适合用户信息管理，树上的每一个节点称为条目，每个条目都存储有若干属性。
* 协议+实现（LDAP+AD）提供了细粒度的访问控制，其解决的核心问题是 **谁 以什么权限 访问什么**。