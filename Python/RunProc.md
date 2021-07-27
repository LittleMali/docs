# 进程操作
在win系统上，如何调用一个exe程序。

## subProcess

```python
import subprocess
exe = "report.exe"
params = "/hello /world"
cmd = exe + " " + params
p = subprocess.Popen(cmd, shell = False)
exeRet = p.wait() # 等待进程运行结束，并获取返回值
```

```python
import subprocess
cmd = "ping www.baidu.com"
p = subprocess.Popen(cmd, shell=False)
ret = p.wait()
print(p.args)
print(p.returncode)
print(ret)
```

如果需要讲进程打印的log保存到文件中，可以接管stdout。
```python
import subprocess

def RunExe(fullCmd, shell, logFile):
    try:
        p = subprocess.Popen(fullCmd, shell=shell, stdout=subprocess.PIPE)
        if logFile:
            # 注意py3中文件open的方式是wb。
            open(logFile, 'wb').write(p.stdout.read())
        else:
            p.stdout.close()
    except Exception as e:
        print("error", e)

cmd = "ping www.baidu.com"
shell = True
logFile = "d:\\1.txt"
RunExe(cmd, shell, logFile)
```

