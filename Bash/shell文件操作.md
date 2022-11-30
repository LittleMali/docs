# shell命令

## 文件操作
* 压缩/解压
```bash
# 压缩
# myfiles --> zip.tar.gz
# -c: create file
# -z: zip or unzip
# -v: verbose， 显示指令执行过程
# -f: 指定备份文件
tar -czvf zip.tar.gz /myfiles

# 解压
# -x: extract， 从备份文件中还原/提取文件
mkdir unzipdir
tar -xzvf zip.tar.gz -C ./unzipdir
```

* 文件重命名  
`rename -n 's/x1/x2/' *.jpg`  
说明： 
  * x1是不想要的字符。
  * x2是替换以后的字符。
  * *.jpg是要替换的文件。
  * -n 表示预览，不会实际执行重命名。
  * -v 表示执行替换。  
举个例子。
```
# 将 .debug 重命名为 .symbol 
# \. 表示转义，这里不转义也可以。
$ rename -n "s/\.debug/.symbol/" *.debug
rename(1.debug, 1.symbol)
rename(2.debug, 2.symbol)
rename(qt.5.12.8.debug, qt.5.12.8.symbol)
rename(qtlib.5.debug, qtlib.5.symbol)

$ rename -v "s/\.debug/.symbol/" *.debug
1.debug renamed as 1.symbol
2.debug renamed as 2.symbol
qt.5.12.8.debug renamed as qt.5.12.8.symbol
qtlib.5.debug renamed as qtlib.5.symbol
```

* 文件大小排序  
`du -s ./* | sort -nr | head -n 5`  
按file size递减排序，展示前5项。

## 文件信息
* 查看lib依赖  
  `ldd xxx.so`
* 查看文件架构  
  `file xxx.so`
* 查看ELF信息  
  * 显示ELF文件头：`readelf -h xx.so`
  * 显示ELF segment信息：`readelf -l xx.so`
  * 显示ELF section信息：`readelf -S xx.so`
```shell
readelf -l ./xxx.so

Elf file type is DYN (Shared object file)
Entry point 0xb5764
There are 11 program headers, starting at offset 64

Program Headers:
  Type           Offset             VirtAddr           PhysAddr
                 FileSiz            MemSiz              Flags  Align
  PHDR           0x0000000000000040 0x0000000000000040 0x0000000000000040
                 0x0000000000000268 0x0000000000000268  R      0x8
  LOAD           0x0000000000000000 0x0000000000000000 0x0000000000000000
                 0x000000000043f1a8 0x000000000043f1a8  R E    0x10000
  GNU_STACK      0x0000000000000000 0x0000000000000000 0x0000000000000000
                 0x0000000000000000 0x0000000000000000  RW     0x10
  NOTE           0x0000000000000270 0x0000000000000270 0x0000000000000270
                 0x0000000000000044 0x0000000000000044  R      0x10
  INTERP         0x00000000003c4ee8 0x00000000003c4ee8 0x00000000003c4ee8
                 0x000000000000001b 0x000000000000001b  R      0x1
      [Requesting program interpreter: /lib/ld-linux-aarch64.so.1]
  GNU_EH_FRAME   0x000000000042bb94 0x000000000042bb94 0x000000000042bb94
                 0x0000000000013614 0x0000000000013614  R      0x4
  LOAD           0x00000000004449b0 0x00000000004549b0 0x00000000004549b0
                 0x0000000000012ef8 0x0000000000016da9  RW     0x10000
  TLS            0x00000000004449b0 0x00000000004549b0 0x00000000004549b0
                 0x0000000000000000 0x0000000000000020  R      0x8
  GNU_RELRO      0x00000000004449b0 0x00000000004549b0 0x00000000004549b0
                 0x000000000000b650 0x000000000000b650  RW     0x8
  DYNAMIC        0x000000000054b000 0x000000000054b000 0x000000000054b000
                 0x00000000000002e0 0x00000000000002e0  RW     0x8
  LOAD           0x000000000054b000 0x000000000054b000 0x000000000054b000
                 0x0000000000038aa0 0x0000000000038aa0  RW     0x1000
```