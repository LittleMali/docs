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
* 切换线程
  ~1 s
* 遍历栈
  ~*kvn

