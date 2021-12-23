# Sqlite3

## 安装
一般情况下,Linux都会预安装SQLite,所以在安装前可以尝试先看一先自己的电脑上有没有安装:
```bash
$ sqlite3
SQLite version 3.28.0 2019-04-15 14:49:49
Enter ".help" for usage hints.
Connected to a transient in-memory database.
Use ".open FILENAME" to reopen on a persistent database.
sqlite> 
```

如果没有安装,可以根据以下步骤进行安装
```bash
sudo apt-get install sqlite3  
```
如果安装后不想使用命令行,可以安装图形化界面
```bash
sudo apt-get install sqlitebrowser  
```

