# windbg 句柄

```shell
# 看句柄信息
!handle 00000080 f

```

## 看句柄泄露
```
!htrace -enable
!htrace -snapshot
g
```