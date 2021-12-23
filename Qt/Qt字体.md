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