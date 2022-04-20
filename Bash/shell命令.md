# shell命令

## 文件操作
* 压缩/解压
```bash
# 将/data压缩成功test.tar.gz
# -c: create file
# -z: zip or unzip
# -v: verbose， 显示指令执行过程
# -f: 指定备份文件
tar -czvf test.tar.gz /data

# 解压
# -x: extract， 从备份文件中还原/提取文件
tar -xzvf test.tar.gz
```
## 小技巧
### gdb attach pid
`gdb attach $(ps -ef | grep process_name | grep -v grep | awk '{print $2}')`
  * ps -ef 显示所有进程。
  * grep process_name 找出包含progress_name的进程，如果此时只有一个叫这个名字的进程的话，那么会显示两个进程：一个是process_name这个进程，一个是grep进程。
  * grep -v grep 过滤掉前一步显示出来的grep进程。
  * awk’{print $2}’ 显示process_name进程信息的第二列数据。  
  **注意**：无法处理多进程的例子。


