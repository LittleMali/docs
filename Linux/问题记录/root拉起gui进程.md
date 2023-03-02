# root拉起gui进程

```c++
// export DISPLAY=$display && nohup $path >/dev/null 2>&1 &

std::string cmd;
cmd.append("export DISPLAY=")
    .append(display)
    .append(" && nohup ")
    .append(pathTmp)
    .append(" ")
    .append(params)
    .append(" > /dev/null 2>&1 &");
auto ret = system(cmd.c_str());
```