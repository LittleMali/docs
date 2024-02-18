# win系统安全-访问控制

windows的访问控制，是控制“谁”可以“访问”某个“文件”。

* “谁”：登录用户的某个进程，准确的说是线程。
* “访问”：对文件是打开，对目录是穿越。允许访问还是拒绝访问。
* “文件”：不仅指文件系统，还包括某些对象的安全性，比如，一个进程句柄。

访问控制是提升系统安全的一个基础屏障，因为病毒、木马等恶意文件，需要落地到文件，或者篡改系统文件，防护住了文件系统，也就保护了系统安全。那么，如何保护文件系统呢，访问控制就是关键，限制对文件的访问权限才能防止文件被篡改，植入，删除，复制等问题。

## 1 DAC简介
Unix操作系统从一开始就发展起了一套DAC（Discretionary Access Control，自主访问控制）技术，Unix系统将用户分为文件主（文件的创建者）、文件主的同组人和其他用户三类，把对文件的访问分为读、写和执行三种基本操作，并规定了每一种用户可以对文件进行什么样的操作，例如，`-rwsr-sr-x`。

在Unix系统上，对于用户的身份，是在用户登录系统时确认的；对于文件的访问权限，是在创建文件时决定的。如果某个用户想偷偷修改一个文件，那么，系统会判断此用户是否有权限执行。三种用户，三种访问方式，只需要9个标记位就可以表示了，对于Unix而言，这是高效且满足需求的。

对于Windows而言，情况就不一样了，在最开始，Windows是单人计算机，没有考虑多用户登录的场景，因此，Windows在设计之初，只有一些简单的标记位来控制文件权限，例如：`FILE_ATTRIBUTE_READONLY`, `FILE_ATTRIBUTE_ARCHIVE`等。

随着技术的发展，无论是Unix还是Windows，这些简单的区分已经无法满足需求了。我们需要对用户和访问权限进行更细致的划分，比如小组A和小组B有财务数据的访问权限，其中仅小组A中的用户甲可以写数据。因此，在DAC的基础上演化出了ACL（ Access Control List，访问控制列表），ACL中详细罗列着哪些用户有权限访问文件，操作系统严格按列表进行判断。目前，Windows和Unix都实现ACL机制。

## 2 用户身份Token
当用户试图访问一个文件时，准确的说是 该用户下某个进程的线程试图访问文件，用户的进程/线程都有一个类似身份标识的token，用于记录此用户的各个属性。而目标文件则有一个ACL，ACL中规定了谁可以访问此文件。因此，操作系统只需要用进程/线程的token和文件的ACL进行对比，就可以确定此次访问行为是否允许。

### 2.1 用户标识SID
如何表明用户身份？

当一个用户登录系统时，操作系统会为其分配唯一的“用户号”，用户号在本机中必须是唯一的，如果是忘了环境，则在本网络中是唯一的。这个唯一的用户号称为SID。
```c++
typedef struct _SID {
  UCHAR                    Revision;
  UCHAR                    SubAuthorityCount;
  SID_IDENTIFIER_AUTHORITY IdentifierAuthority; // 简称IA
  ULONG                    SubAuthority[ANYSIZE_ARRAY]; // 简称SA
} SID, *PISID;
```
IdentifierAuthority是6字节的数组，也可以视为48b整数，其表示此SID是由谁授予的，相当于发证机关。
```
SECURITY_NULL_SID_AUTHORITY	    0
SECURITY_WORLD_SID_AUTHORITY	1
SECURITY_LOCAL_SID_AUTHORITY	2
SECURITY_CREATOR_SID_AUTHORITY	3
SECURITY_NON_UNIQUE_AUTHORITY	4
SECURITY_NT_AUTHORITY	        5
```

SubAuthority是一个数组，相当于发证机关的下属部门，也可以用于描述一些属性，可以理解为，数组唯一确定了一个用户或者用户组,也称为RID（Relative Identifier）。

SID的值都存储在注册表中，其表示形式为 字符S 开头的“S-R-IA-SA”，例如，“S-1-5-32-500”。
* 1：版本号。
* 5：发证机关， SECURITY_NT_AUTHORITY 。
* 32-500：32是 SECURITY_BUILTIN_DOMAIN_RID，500是DOMAIN_USER_RID_ADMIN，这里表示系统管理员。如果是一般用户账号，往往从1000开始分派。比如，1028，表示是第29个用户。

Windows定义了一些特殊的SID，用来表示某个集合。
* S-1-1-0：表示任何用户，SA是0，表示任意。
* S-1-2-0：SA是0，表示本机登录的用户的集合。
* S-1-5-6，SA是SECURITY_SERVICE_RID，表示所有 服务线程 的集合。

### 2.2 Token
一个用户可以用多种不同的属性，同时，一个用户也可以属于几个不同的用户组，因此，在描述用户信息的Token中，不仅要包括用户本身的SID，还要包括所属用户组的SID的。

Token的数据结构定义如下。

```c++
typedef struct _TOKEN
{
     TOKEN_SOURCE TokenSource;
     LUID TokenId; // 编号，一个64b整数
     LUID AuthenticationId;
     LUID ParentTokenId;
     LARGE_INTEGER ExpirationTime;
     PERESOURCE TokenLock;
     LUID ModifiedId;
     SEP_TOKEN_PRIVILEGES Privileges;
     SEP_AUDIT_POLICY AuditPolicy;
     ULONG SessionId;
     ULONG UserAndGroupCount;  //
     ULONG RestrictedSidCount;  //
     ULONG VariableLength;
     ULONG DynamicCharged;
     ULONG DynamicAvailable;
     ULONG DefaultOwnerIndex;
     PSID_AND_ATTRIBUTES UserAndGroups;
     PSID_AND_ATTRIBUTES RestrictedSids;
     PVOID PrimaryGroup;
     ULONG * DynamicPart;
     PACL DefaultDacl;
     TOKEN_TYPE TokenType; //
     SECURITY_IMPERSONATION_LEVEL ImpersonationLevel;
     ULONG TokenFlags;
     UCHAR TokenInUse;
     ULONG IntegrityLevelIndex;
     ULONG MandatoryPolicy;
     PSECURITY_TOKEN_PROXY_DATA ProxyData;
     PSECURITY_TOKEN_AUDIT_DATA AuditData;
     PSEP_LOGON_SESSION_REFERENCES LogonSession;
     LUID OriginatingLogonSession;
     SID_AND_ATTRIBUTES_HASH SidHash;
     SID_AND_ATTRIBUTES_HASH RestrictedSidHash;
     ULONG VariablePart;
} TOKEN, *PTOKEN;
```

Token是进程EPROCESS的结构，进程的数据结构中有EX_FAST_REF指针指向TOKEN变量。因此，多个进程的token可能指向了同一个数据结构。事实上，一个用户登录以后，用户名下的进程可能都挂着同一个token。这是因为在创建进程时，默认会从父进程继承其token。

## 名词汇总
| 缩写 | 中文         | 英文                         |
| ---- | ------------ | ---------------------------- |
| DAC  | 自主访问控制 | Discretionary Access Control |
| ACL  | 访问控制列表 | Access Control List          |



访问控制模型：Access Control Model
访问令牌：Access Token
安全描述符：Security Descriptor

discretionary access control list：DACL
system access control list： SACL
LSA：Local Security Authority