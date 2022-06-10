# shell命令

## 进程信息
* 查看进程列表  
`ps -aux | grep qq`  
![20220608144829](https://raw.githubusercontent.com/LittleMali/docs/master/mdPics/20220608144829.png)
## 小技巧

### gdb attach pid
`gdb attach $(ps -ef | grep process_name | grep -v grep | awk '{print $2}')`
  * ps -ef 显示所有进程。
  * grep process_name 找出包含progress_name的进程，如果此时只有一个叫这个名字的进程的话，那么会显示两个进程：一个是process_name这个进程，一个是grep进程。
  * grep -v grep 过滤掉前一步显示出来的grep进程。
  * awk’{print $2}’ 显示process_name进程信息的第二列数据。  
  **注意**：无法处理多进程的例子。
