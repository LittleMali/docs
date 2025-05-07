# intel VTune工具

## 安装&安装
离线安装包：https://www.intel.com/content/www/us/en/developer/tools/oneapi/base-toolkit-download.html?packages=oneapi-toolkit&oneapi-toolkit-os=linux&oneapi-lin=offline

下载以后
chmod +x intel-oneapi-base-toolkit-2025.1.1.36_offline.sh
./intel-oneapi-base-toolkit-2025.1.1.36_offline.sh

## 运行
```shell
source /opt/intel/oneapi/setvars.sh  # 加载环境变量
vtune-gui                            # 启动图形界面
```