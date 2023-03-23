pretty-printer可以让gdb调试的时候，以python的风格显示STL，极大的方便调试。

可以通过配置文件开启gdb的pretty-printer功能，新建 ~/.gdbinit 文件，如果提权以root权限调试，需要在root目录下拷贝一份改文件，内容如下:


```bash
set print pretty on
set print object on
set print static-members on
set print demangle on
set print sevenbit-strings off

python
import sys
sys.path.insert(0, '/usr/share/gcc-10/python')
from libstdcxx.v6.printers import register_libstdcxx_printers
register_libstdcxx_printers (None)
end
```

注意 '/usr/share/gcc-10/python' 修改为本机的对应目录