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