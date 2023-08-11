# Qt边框设置

## 如何设置border
```c
/*
      whole wnd size
|-----------------------------|
|    i am shadow area         |
|  |------i am border------|  |
|  |                       |  |
|  |     client  area      |  |
|  |       (Qt widgets)    |  |
|  |                       |  |
|  |                       |  |
|  |-----------------------|  |
|     |
| --- |
*/
```
Qt的窗口是如上所示，设置边框和阴影的几个关键点如下。
1. setFixedSize设置整个窗口size。
2. 设置shadow或者border size。
3. setContentsMargins把shadow或者border显示出来。
4. 不要轻易重载paintEvent，paintEvent会重绘整个窗口，覆盖stylesheet效果。

有几个细节可以注意下。
1. setGeometry和setFixedSize都是设置整个窗口的大小。
2. 把四周留白才能显示出来边框和阴影。
3. 阴影效果会覆盖边框效果。

## border显示不出来的问题
### QWidget不支持border
如果控件/窗口是派生自QWidget的，那么无论怎么设置setStyleSheet都不行。  
解决方案是有两个。  
1. 直接从QFrame派生控件。QFrame是支持设置border的。
2. 复写QWidget的paintEvent，自己画一个边框。
   不建议复写paintEvent，因为paintEvent是重绘整个QWidget，包括背景图等，这会导致为QWidget设置的background被覆盖。
```c++
class MyWidget : public QWidget
{
protected:
    void paintEvent(QPaintEvent *event) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setBrush(QColor(0xFFFFFF)); // 设置背景
        
        painter.setPen(QPen(Qt::red, 1)); // 设置边框颜色和宽度
        painter.drawRect(0, 0, width() - 1, height() - 1); // 绘制边框
    }
};
```
   
### 没有设置setContentMargins。