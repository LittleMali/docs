# 高DPI
## 原理
显示器的物理dpi。
操作系统的逻辑dpi，默认96，表示100%，无缩放。

1. os显示5号字体 -- 假定标准为占用1英寸的大小 -- 对应逻辑dpi，96个像素点。
2. 物理显示器 -- 占用96个像素点 -- 因为是高清屏，所以像素点很密，那么96个像素点看起来只有0.5英寸。
3. 在100%缩放下，显示器上的5号字体，看起来比预期的要小。
4. 所以，要放大显示器上的5号字体，就要让5号字体占用更多的像素点。
5. 将os的dpi设置为200%，os显示时，5号字体继续按标准占用1英寸大小，但是1英寸的像素点变成了96*2=192。
6. 因此，实际显示器展示5号字体时，要占用192个像素点，这看起来就大了。

最后我们再讨论下Win8.1 对高DPI的支持, WIn8.1对高DPi以3种方式支持  Process_DPI_Awareness ： 
typedef enum _Process_DPI_Awareness { 
  Process_DPI_Unaware            = 0,
  Process_System_DPI_Aware       = 1,
  Process_Per_Monitor_DPI_Aware  = 2
} Process_DPI_Awareness;

第一种Unaware, 该种方式是告诉系统， 我的程序不支持DPI aware, 请通过DWM虚拟化帮我们实现。 该方式和上面Win7/Win8对高DPI的支持的实现基本一样，主要区别是它通过GetWindowRect取到的坐标都是经过DWM缩放后的， 无论对方窗口是不是支持DWM虚拟化。

第二种方式是System DPI aware， 该方式下告诉系统， 我的程序会在启动的显示器上自己支持DPI aware, 所以不需要对我进行DWM 虚拟化。 但是当我的程序被拖动到其他DPI不一样的显示器时， 请对我们先进行system DWM虚拟化缩放。

第三种方式是Per Monitor DPI aware, 该方式是告诉系统， 请永远不要对我进行DWM虚拟化，我会自己针对不同的Monitor的DPi缩放比率进行缩放。