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

