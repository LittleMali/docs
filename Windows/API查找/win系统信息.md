# win系统信息
本文介绍一下如何获取系统的各个信息。

## 系统默认语言设置
### Locale和LCID
Locale是指特定于某个国家或地区的一组设定，包括字符集，数字、货币、时间和日期的格式等。在Windows中，每个Locale可以用一个32位数字表示，记作LCID。
通过语言设置，我们可以判断当前系统是中文环境还是英文环境。
一种方法是获取LCID，进行判断。
``` c++
// 获取系统Locale Id。
LCID GetSystemDefaultLCID();

//
//  A locale ID is a 32 bit value which is the combination of a
//  language ID, a sort ID, and a reserved area.  The bits are
//  allocated as follows:
//
//       +-------------+---------+-------------------------+
//       |   Reserved  | Sort ID |      Language ID        |
//       +-------------+---------+-------------------------+
//        31         20 19     16 15                      0   bit
//
//  Locale ID creation/extraction macros:
//    MAKELCID            - construct the locale id from a language id and a sort id.
//    MAKESORTLCID        - construct the locale id from a language id, sort id, and sort version.
//    LANGIDFROMLCID      - extract the language id from a locale id.
//    SORTIDFROMLCID      - extract the sort id from a locale id.
//    SORTVERSIONFROMLCID - extract the sort version from a locale id.
```

另外一个方法是直接获取语言集。
``` c++
LANGID GetSystemDefaultLangID();
LANGID GetUserDefaultLangID();

//
//  A language ID is a 16 bit value which is the combination of a
//  primary language ID and a secondary language ID.  The bits are
//  allocated as follows:
//
//       +-----------------------+-------------------------+
//       |     Sublanguage ID    |   Primary Language ID   |
//       +-----------------------+-------------------------+
//        15                   10 9                       0   bit
//
//
//  Language ID creation/extraction macros:
//
//    MAKELANGID    - construct language id from a primary language id and
//                    a sublanguage id.
//    PRIMARYLANGID - extract primary language id from a language id.
//    SUBLANGID     - extract sublanguage id from a language id.
```
对系统语言的判断，主要是通过Primary和Sublanguage两个字段。
Primary language用来表示主要语言是什么，比如中文，英文。
Sublanguage被用来区分同一种语言的不同编码，比如中文下的简体和繁体。
```
#define LANG_CHINESE 0x04
#define LANG_ENGLISH 0x09
#define LANG_FRENCH 0x0c
#define LANG_GERMAN 0x07

#define SUBLANG_CHINESE_TRADITIONAL 0x01 // Chinese (Taiwan Region)
#define SUBLANG_CHINESE_SIMPLIFIED 0x02 // Chinese (PR China)
#define SUBLANG_ENGLISH_US 0x01 // English (USA)
#define SUBLANG_ENGLISH_UK 0x02 // English (UK)
```
因此，在判断系统当前语言时，可以GetSystemDefaultLangID获取语言集，再判断返回值的primary language是否等于0x04.

### 代码页
每个Locale都联系着很多信息，其中最重要的信息就是字符集了，即Locale对应的语言文字的编码。Windows将字符集称作代码页。
每个Locale可以对应一个ANSI代码页和一个OEM代码页。Win32 API使用ANSI代码页，底层设备使用OEM代码页，两者可以相互映射。
例如English (US)的ANSI和OEM代码页分别为“1252 (ANSI - Latin I)”和“437 (OEM - United States)”。 Chinese (PRC)的ANSI和OEM代码页都是“936 (ANSI/OEM - Simplified Chinese GBK)”。 Chinese (TW)的ANSI和OEM代码页都是“950 (ANSI/OEM - Traditional Chinese Big5)”。

### 系统Locale和用户Locale
在Windows中，通过控制面板可以为系统和用户分别设置Locale。系统Locale决定代码页，用户Locale决定数字、货币、时间和日期的格式。
使用GetSystemDefaultLCID函数和GetUserDefaultLCID函数分别得到系统和用户的LCID。
GetSystemDefaultUILanguage 和GetUserDefaultUILanguage得到的是您当前使用的Windows版本所带的UI资源的语言。
用户程序缺省使用的代码页是当前系统Locale的ANSI代码页，可以称作ANSI编码，也就是A版本的Win32 API默认的字符编码。对于一个未指定编码方式的文本文件，Windows会按照ANSI编码解释。

我们看一下实际的例子。
![20210917155922](https://raw.githubusercontent.com/LittleMali/docs/master/mdPics/20210917155922.png)
在上面的图片中，控制面板--时钟和区域--格式，默认是中文，点开“语言首选项”，对应的也是中文。

1. 修改为英文，看看会发生什么？
   ![20210917160157](https://raw.githubusercontent.com/LittleMali/docs/master/mdPics/20210917160157.png)
   当修改格式为英文时，我们仅修改了当前用户的设置，因此，几个函数返回值如下。
    GetSystemDefaultLangID=2052, primary=4, sub=2
    GetUserDefaultLangID=2057, primary=9, sub=2
    GetSystemDefaultUILanguage=2052, primary=4, sub=2
2. 再修改一处为英文，看看是否有变化。
   ![20210917160402](https://raw.githubusercontent.com/LittleMali/docs/master/mdPics/20210917160402.png)
   在我们注销重新登录之前，几个函数返回值并没有发生变化。
    GetSystemDefaultLangID=2052, primary=4, sub=2
    GetUserDefaultLangID=2057, primary=9, sub=2
    GetSystemDefaultUILanguage=2052, primary=4, sub=2

    那我们注销重新登录一下，几个函数返回值还是没有发生变化，但是windows的ui已经从中文变成了英文。
    GetSystemDefaultLangID=2052, primary=4, sub=2
    GetUserDefaultLangID=2057, primary=9, sub=2
    GetSystemDefaultUILanguage=2052, primary=4, sub=2
    ![20210917160836](https://raw.githubusercontent.com/LittleMali/docs/master/mdPics/20210917160836.png)

3. 到这里，我们梳理一下，修改 控制面板--时钟和区域--格式里的中英文，影响的是日期，时间，货币等的显示，同时影响了GetUserDefaultLangID的返回值。那如果修改的是 格式--首选项，再打开“语言”，修改为中英文，影响的是win UI的展示，不会影响GetUserDefaultLangID的结果。
4. 我们要修改系统级的语言设置，才会影响GetSystemDefaultLangID的返回结果。
   ![20210917161716](https://raw.githubusercontent.com/LittleMali/docs/master/mdPics/20210917161716.png)
   ![20210917161817](https://raw.githubusercontent.com/LittleMali/docs/master/mdPics/20210917161817.png)
   当我们把系统语言从中文改成英文(加拿大)时，函数的返回结果如下。请注意，我已经将“格式”里面的语言改回中文了。
    GetSystemDefaultLangID=4105, primary=9, sub=4
    GetUserDefaultLangID=2052, primary=4, sub=2
    GetSystemDefaultUILanguage=2052, primary=4, sub=2
5. 至于GetSystemDefaultUILanguage这个函数怎么用，没看明白，至少，我们的修改并未影响到这个函数。