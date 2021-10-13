# python操作ini文件

## ini文件操作
python支持对ini文件的操作，import ConfigParser就可以支持ini了。需要注意的是py2和py3的差异，如下。
``` python
import configparser # python3
import ConfigParser # python2
```

### python2的大小写问题
python2的ConfigParser在写文件时，对于section下的item是不区分大小写的。
```
[sectionA]
name=LittleMa
```
在这个例子中，即使传入的参数是Name，但是write文件以后会变成name。那么，如果要保持传入参数的大小写，要自己设置ConfigParser的属性。
``` python
class MyConfigParser(ConfigParser.ConfigParser):
    # set ConfigParser options for case sensitive.
    def __init__(self, defaults=None):
        ConfigParser.ConfigParser.__init__(self, defaults=defaults)
 
    def optionxform(self, optionstr):
        return optionstr

config = MyConfigParser()
config.read(config_path)
```

### ini文件读取
```python
import configparser # note py3 !!!
import os

curpath = os.path.dirname(os.path.realpath(__file__))
cfgpath = os.path.join(curpath, "cfg.ini")
print(cfgpath)  # cfg.ini的路径
 
conf = configparser.ConfigParser()
conf.read(cfgpath, encoding="utf-8")  # python3
# conf.read(cfgpath)  # python2
 
# 获取所有的section
sections = conf.sections()
print(sections)  # 返回list
 
items = conf.items('email_163')
print(items)  # list里面对象是元祖
```

### ini文件删除
```python
# 删除一个 section中的一个 item（以键值KEY为标识）
conf.remove_option('email_163', "port")
# 删除整个section
conf.remove_section('email_163')
```

### ini文件添加
```python
# 添加一个select
conf.add_section("emali_tel")

# 往select添加key和value
conf.set("emali_tel", "sender", "yoyo1@tel.com")
conf.set("emali_tel", "port", "265")
```

### ini文件保存
```python
conf.write(open(cfgpath, “w”))  # 删除原文件重新写入
conf.write(open(cfgpath, “a”))  # 追加模式写入
```

### ini文件更新
通常情况下，我们更多的还是修改ini里面某个item的值。
如果要写入中文的，py脚本的编码方式要指定为utf-8。
```python
# coding:utf-8
import configparser
import os
 
curpath = os.path.dirname(os.path.realpath(__file__))
cfgpath = os.path.join(curpath, "cfg.ini")
 
conf = configparser.ConfigParser()
 
# 先读出来
conf.read(cfgpath, encoding="utf-8")
 
# 修改section里面的值
conf.set("email_163", "port", "中文")  # 写入中文
 
conf.write(open(cfgpath, "r+", encoding="utf-8"))  # r+模式
```