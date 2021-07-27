# 文件操作

## 删除文件
删除单个文件
```python
if os.path.exists(file):
    os.remove(file)
```

删除文件夹
如果dir为空，可以直接rmdir掉。
```python
# dir must be empty
if os.path.exists(dir):
    os.rmdir(dir)
```
如果dir非空，rmtree更简单一些。
```python
import shutil
shutil.rmtree(dir)
```

