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

## 遍历文件
``` python
import os
import shutil
allFiles = os.listdir(dir)
for file in allFiles:
    curFullFile = os.path.join(dir, file)
    if os.path.isdir(curFullFile):
        print("this is a dir")
        continue
    if os.path.exists(curFullFile):
        dstFile = "xxx"

        try:
            shutil.copyfile(curFullFile, dstFile)
        except Exception as e:
            print("copyfile failed")
```
