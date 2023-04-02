# Qt设置字体属性
## Qt设置字体大小
```c++
QLabel *lable = new QLabel("系统已就绪  ", this);
QFont font;
font.setPointSize(12); //字体大小
lable->setFont(font); //其他控件一样
```

## Qt设置颜色
方法1：使用调色板。
```c++
QLabel *lable = new QLabel("系统已就绪  ", this);
QPalette pe;
pe.setColor(QPalette::WindowText, Qt::white);//设置颜色
lable->setPalette(pe);
```
方法2：使用样式。
```c++
QLabel *lable = new QLabel("系统已就绪  ", this);
lable->setStyleSheet("color:red;");
```

## Qt设置颜色和字体
```c++
QLabel *lable = new QLabel("系统已就绪  ", this);
lable->setStyleSheet("color: black;font-size:13px");
```

## Qt设置文本换行
```c++
QLabel *lable = new QLabel(this);
lable->setStyleSheet("color: black;font-size:13px");
label->setFixedWidth(100);

QFontMetrics fontPath(lable->font());
// width * 2 表示最大可以显示两行。
fontPath.elidedText("我是一个很长很长的文案", Qt::ElideRight, lable->width() * 2);
label->setText(fontPath);
```

## Tips
关于设置Qt::FramelessWindowHint的窗口，界面不能自主刷新的问题。
问题：如果一个窗口设置了无边框Qt::FramelessWindowHint，则该窗口界面不会自动刷新。

解决办法：重载showEvent,代码如下：
```c++
void MyWidget::showEvent(QShowEvent *event)
{
this->setAttribute(Qt::WA_Mapped);
QWidget::showEvent(event);
}
```
注：这个办法是从其他人的博客发现的，在此说明一下。