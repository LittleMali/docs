# Message

## 用法
`message([<mode>] "message text" ...)`
mode 的值包括 FATAL_ERROR、WARNING、AUTHOR_WARNING、STATUS、VERBOSE等。
* STATUS：最常用的命令，常用于查看变量值，类似于编程语言中的 DEBUG 级别信息。
* "message text"为显示在终端的内容。

例如：
`message(STATUS "result:${RET}")`
`message(STATUS "PLATFORM_PROCESSOR: ${PLATFORM_PROCESSOR}")`
