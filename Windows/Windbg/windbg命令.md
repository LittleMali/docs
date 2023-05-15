# windbg命令

## 时间
* dmp生成的时间
  .time
## 查看耗时
!runaway

## 查看内存
dt 0x1234 ModuleName!Class

* 查看当前栈变量
dt /v /t
![picture 1](../../images/386cab651f90dcbd19af3ce2d74629c8dd2032ff3401ae21f16ab778d2344434.png)  

## 线程
* 当前线程
  ~.
* 通过线程id切换线程，例如，切换到1号线程
  ~1 s
* 遍历栈
  ~*kvn
* 通过线程地址查看线程信息
```
0:000:x86> ~~[0x000005f4]
   3  Id: aa0.5f4 Suspend: -1 Teb: 7efd5000 Unfrozen
      Start: iOAEntBiz!CTPNEvtMgr::Thread2WatchEvent (0040e619)
      Priority: 0  Priority class: 32  Affinity: 3
```
* 通过线程地址切换线程。
```
0:000:x86> ~~[0x000005f4]s
ntdll_77980000!NtWaitForSingleObject+0x15:
7799f901 83c404          add     esp,4
```

## 32位切换
.load wow64exts
!sw

## 清空屏幕
.cls

## 查看临界区
* 显示所有临界区信息
  !cs
* 只显示死锁的临界区
  !cs -l
* 查看某个临界区的栈
  !cs -o 0x000005f4