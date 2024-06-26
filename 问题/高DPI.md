# 高DPI
## 原理
显示器的物理dpi。
操作系统的逻辑dpi，默认96，表示100%，无缩放。

1. os显示5号字体 -- 5号字体占用1英寸大小（标准规定） -- 对应逻辑dpi，96个像素点。
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

## 实践
| Test.exe     | CreateWindow大小 | 系统dpi | 系统显示的大小 |
| ------------ | :--------------: | :-----: | :------------: |
| dpi unaware  |   100 * 200px    |  200%   |  200 * 400px   |
| system aware |   100 * 200px    |  200%   |  100 * 200px   |

在上面的例子中，Test.exe是dpi unaware，表示由系统进行dpi缩放，Test.exe创建窗口是100 * 200px，但是系统会缩放为200 * 400px，这个放大行为对进程是无感的，进程依旧认为自己是100 * 200px大小，也就是说Test.exe调用GetWindowRect获取自身窗口大小时，拿到的是100 * 200px。

如果Test.exe是system aware，那么，系统就不会帮忙缩放窗口大小，系统认为Test.exe会自己处理缩放问题，所以，Test.exe创建了100 * 200px的窗口，系统就绘制了一个100 * 200px窗口，这就是在高分屏下窗口看起来很小的原因。因此，在Test.exe中，它应该先获取dpi设置，再依据缩放去调整大小，在这个例子中，应该将100 * 200px调整为200 * 400px。如果Test.exe调用GetWindowRect获取自身窗口大小时，它拿到的是实际显示的大小，只是，一个显示正常的ui窗口，其窗口大小已经被调整过了，调整为200 * 400px，因此，GetWindowRect的结果是200 * 400px。



## 实践2

| 系统dpi | WndTest.exe  | CreateWindow |   显示大小   | spy++抓取    |
| :-----: | ------------ | :----------: | :----------: | ------------ |
|  200%   | dpi unaware  | 500 * 300px  | 1000 * 600px | 1000 * 600px |
|  200%   | system aware | 500 * 300px  | 500 * 300px  | 500 * 300px  |


GetWindowRect获取的的结果与调用进程的dpi相关。如下表所示，WndReader.exe去读取自身创建的窗口，也读取WndTest.exe创建的窗口。

1. 先看WndTest，系统显示的大小跟WndTest的dpi相关。
2. GetWindowRect获取的显示大小，是dpi缩放以后的大小。
3. unaware WndTest显示大小是1000 * 600px，unware WndReader去读取时，拿到的是1000 * 600px，还要再除以dpi，所以，最终结果是500 * 300px。
4. unaware WndTest显示大小是1000 * 600px，aware WndReader去读取时，拿到的是1000 * 600px，此时，WndReader已经自己处理了dpi，所以，系统不再帮忙缩放，因此，获取的最终结果就是1000 * 600px。

| 系统dpi | WndTest.exe  | CreateWindow | WndReader.exe | CreateWindow | 读取自身大小 | 读取WndTest大小 |
| :-----: | ------------ | :----------: | ------------- | :----------: | ------------ | --------------- |
|  200%   | dpi unaware  | 500 * 300px  | dpi unaware   | 500 * 300px  | 500 * 300px  | 500 * 300px     |
|  200%   | system aware | 500 * 300px  | dpi unaware   | 500 * 300px  | 500 * 300px  | 250 * 150px     |
|  200%   | dpi unaware  | 500 * 300px  | system aware  | 500 * 300px  | 500 * 300px  | 1000 * 600px    |
|  200%   | system aware | 500 * 300px  | system aware  | 500 * 300px  | 500 * 300px  | 500 * 300px     |

## 实践3
假设dpi是200%，src进程是unware。
* win10下，不管目标进程是什么aware，src获取目标窗口都是除2的大小 - GetWindowRect。
* win10下，GetCursorPos 拿到的是除2的坐标。
* win7下，GetWindowRect拿到的是 显示大小，是200%的大小，不管目标窗口是什么aware。
* win7下，GetCursorPos拿到的是100%下的坐标，是除2的坐标。
* win7下，如果仅设置dpi=125%，那么GetWindowRect拿到的是100%的大小，win7系统对于dpi<=125%的设置，GetWindowRect拿到的是100%的大小，这是win7对高dpi的支持并不完善引起的。

