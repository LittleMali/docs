# win内存泄露

## 工具
* umdh.exe
* gflags.exe


## umdh检测内存泄露

1. 开启gflag标记。
2. umdh抓取一次进程快照。
3. undh抓取第二次进程快照。
4. 两次进行比对，看内存差异。

gflag标记如下：
![picture 1](../images/6f26c343ea37d515b7d2d94ba749c294eb67b14fd812c8141741d69993ce254c.png)  
