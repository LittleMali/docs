# 入门

## 基础命令
* 附加调试  
`gdb attach %pid%`

* 查看core-dmp  
`gdb %proc_filepath% %dmp_filepath%`  
例如：`gdb /home/little/test /tmp/core-tmp-1234`

## 调试命令
### 线程
* 查看所有线程  
  `info thread`
* 切换线程  
  `thread 1234`

### 函数栈
* 查看当前栈  
  `bt`
* 切换栈  
  `frame 1`
* 函数参数  
  `info args`
* 函数局部变量  
  `info locals`
* 全局和静态变量  
  `info variables`

### 变量
* 查看变量  
`print this`
`print this->m_flag`
`print *this`

### 传参
调试某个本地程序时，如何给程序传入参数。
1. 在启动 GDB 时传递参数：在启动 GDB 时，可以将程序的命令行参数直接传递给 GDB。  
   gdb myprogram --args arg1 arg2
2. 在 GDB 内部设置参数：在 GDB 命令行中，你可以使用 set args 命令来设置程序的命令行参数。  
   set args arg1 arg2
3. 在 GDB 运行时传递参数：在 GDB 运行时，你可以使用 run 命令来启动程序，并在命令行中传递参数。  
   run arg1 arg2