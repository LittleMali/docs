# elf文件格式检查

Linux系统中有一些工具可以校验ELF文件格式是否合法。

## readelf
readelf是一个命令行工具，可以显示ELF文件的头部、节表、符号表等信息。如果ELF文件格式不正确，readelf将会显示错误信息。

## objdump
objdump是一个命令行工具，可以显示ELF文件的汇编代码、符号表、重定位表等信息。如果ELF文件格式不正确，objdump将会显示错误信息。

## elflint
elflint是一个命令行工具，可以检查ELF文件的格式是否符合标准。如果ELF文件格式不正确，elflint将会显示错误信息。

## checksec
checksec是一个命令行工具，可以检查ELF文件的安全性。如果ELF文件格式不正确，checksec将会显示错误信息。