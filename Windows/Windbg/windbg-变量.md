# windbg变量

```shell
## 先看栈，注意，前面的数字是可以点击的
0:008> kvn
 # ChildEBP RetAddr  Args to Child              
00 1129f824 6fd70e7e 164c3300 2ffcfbc1 6fe2f6cc

## 点击数字，可以看当前栈变量
0:008> dx Debugger.Sessions[0].Processes[1240].Threads[5924].Stack.Frames[0].SwitchTo();dv /t /v
Debugger.Sessions[0].Processes[1240].Threads[5924].Stack.Frames[0].SwitchTo()
1129f82c          class CCommonCfg * this = 0x164c3300
1129f830          class rapidxml::xml_node<char> * root = 0x2ffcfbc1
1129f834          bool bQuery = true
1129f810          class rapidxml::xml_node<char> * node = 0x2ffcfb85
1129f830          class ATL::CStringT<wchar_t,ATL::StrTraitATL<wchar_t,ATL::ChTraitsCRT<wchar_t> > > strProcessList = 

## 或者用dv
0:008> dv
           this = 0x164c3300
           root = 0x2ffcfbc1
         bQuery = true
           node = 0x2ffcfb85
 strProcessList = class ATL::CStringT<wchar_t,ATL::StrTraitATL<wchar_t,ATL::ChTraitsCRT<wchar_t> > >

## dv显示的有些是超链接，可以直接点击的，比如点击this
## this的类型是 (Plugin!CCommonCfg *)，用dx可以查看。
0:008> dx -r1 ((Plugin!CCommonCfg *)0x164c3300)
((Plugin!CCommonCfg *)0x164c3300)                 : 0x164c3300 [Type: CCommonCfg *]
    [+0x000] m_oLocationCfg   [Type: CCommonCfg::CLocationCfg]
    [+0x030] m_oQueryFilter   [Type: CCommonCfg::CQueryFilter]
    [+0x03c] m_oWhiteFilter   [Type: CCommonCfg::CWhiteFilter]
    [+0x060] m_oSpecialKill   [Type: CCommonCfg::CSpecialKill]
    [+0x06c] m_oCustomKill    [Type: CCommonCfg::CCustomKill]
    [+0x078] m_oMustQueryCloud [Type: CCommonCfg::CMustQueryCloud]
    [+0x084] m_cs             [Type: ATL::CComAutoCriticalSection]
    [+0x09c] m_strFileMd5     : "ddc1518ea987b71fd57fb32408b0daea" [Type: ATL::CStringT<wchar_t,ATL::StrTraitATL<wchar_t,ATL::ChTraitsCRT<wchar_t> > >]

## 再继续看某个成员变量，是相对于基址的偏移，所以，0x164c3300 + 0x03c
## 可以用dx看，也可以点超链接。
0:008> dx -r1 (*((Plugin!CCommonCfg::CWhiteFilter *)0x164c333c))
(*((Plugin!CCommonCfg::CWhiteFilter *)0x164c333c))                 [Type: CCommonCfg::CWhiteFilter]
    [+0x000] m_listProcess    : { size=0x12909 } [Type: std::list<ATL::CStringT<wchar_t,ATL::StrTraitATL<wchar_t,ATL::ChTraitsCRT<wchar_t> > >,std::allocator<ATL::CStringT<wchar_t,ATL::StrTraitATL<wchar_t,ATL::ChTraitsCRT<wchar_t> > > > >]
    [+0x00c] m_listClass      : { size=0xe85 } [Type: std::list<ATL::CStringT<wchar_t,ATL::StrTraitATL<wchar_t,ATL::ChTraitsCRT<wchar_t> > >,std::allocator<ATL::CStringT<wchar_t,ATL::StrTraitATL<wchar_t,ATL::ChTraitsCRT<wchar_t> > > > >]
    [+0x018] m_listCombination : { size=0x5f7c } [Type: std::list<CCommonCfg::CBaseInfo,std::allocator<CCommonCfg::CBaseInfo> >]

## m_listProcess 是一个vector，size表示其元素。
```