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
