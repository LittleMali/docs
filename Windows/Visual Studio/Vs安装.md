# 问题记录
## 打开vs时提示缺少组件
本机安装了多个vs版本，vs05，vs10，vs15，卸载vs15后再运行vs05，弹框提示无法加载组件“Microsoft Visual J# ...”。
尝试重新安装vs05无效，后发现删除vs15的残留文件夹即可。

 VS版本             | 应该删除的文件夹
-------------------|--------------
Visual Studio 2013 | %localappdata%\Microsoft\VisualStudio\12.0\ComponentModelCache
Visual Studio 2015 | %localappdata%\Microsoft\VisualStudio\14.0\ComponentModelCache
Visual Studio 2017 | %localappdata%\Microsoft\VisualStudio\15.0_xxxx\ComponentModelCache
