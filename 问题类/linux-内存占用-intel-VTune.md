# intel VTune工具

## 安装&安装
离线安装包：https://www.intel.com/content/www/us/en/developer/tools/oneapi/base-toolkit-download.html?packages=oneapi-toolkit&oneapi-toolkit-os=linux&oneapi-lin=offline

下载以后
chmod +x intel-oneapi-base-toolkit-2025.1.1.36_offline.sh
./intel-oneapi-base-toolkit-2025.1.1.36_offline.sh

## 运行
```shell
source /home/kingsoft/intel/oneapi/setvars.sh  # 加载环境变量

echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope
echo 0 | sudo tee /proc/sys/kernel/kptr_restrict

vtune-gui                            # 启动图形界面
```

配置symbols路径。
![picture 0](../images/30d6c4a2ee74b32aa3df6fc56328cd7923e20094579ceaf5cae94a9db439820a.png)  
