# Qt样式
 Qt样式是指通过setStyleSheet API来设置控件样式。
 一种方法是针对单个控件的设置，另一种方法是利用qss文件针对整个窗口设置。

 ## 单个控制设置样式
 针对单个控件，可以在代码里面直接调用setStyleSheet设置，也可以利用QtCreator在ui界面编辑器里面设置。  
 这种方法使用起来简单。
 ```c++
 QWidget* pMainWidgt = new QWidget(this);
pMainWidgt->setFixedSize(400, 200);
pMainWidgt->setObjectName("MainWidget");
pMainWidgt->setStyleSheet("#MainWidget {border:1px solid red;}");

QLabel* pTitle = new QLabel();
pTitle->setStyleSheet("font-size:20px; color:#000000");
pTitle->setText("标题");
 ```

## qss文件设置样式
假设我们有换肤的需求，如果定制N套不同的皮肤，那么，使用代码控制单个控件的样式，就会显得十分麻烦。  
其次，如果我们将样式放在ui文件中，使用不同的ui文件编译二进制，比代码修改要稍好，但是不够直观。  
**答案**：利用qss文件统一界面样式，将窗口及其子控件的样式，全部聚集到qss文件，如果设计够好，换肤就只是更换一份qss文件和配套图片等资源。  

* 编写qss文件。  
qss文件示例。
```
/* qss样式文件示例 MainWnd.qss */
MainWnd {

}
MainWnd QLabel[qfont="CESI"] {
    font: 14pt "CESI黑体";
}
MainWnd QLabel[qcolor="blue"] {
    color: rgb(0, 85, 255);
}

/* 这是合法的注释 */
// 这是非法的注释

```
* 将qss文件添加到qrc资源。  
我们在实际加载qss文件时，可以通过qrc资源去引入，也可以使用绝对路径。  
注意，如果试图使用相对路径，那么需要小心是相对谁的路径。编译&调试进程时，是相对调试exe的；打包安装时， 是相对安装目录的。因此，使用qrc资源引入qss文件，路径依赖上面没有什么坑。
```
<RCC>
    <qresource prefix="/qss">
        <file alias="MainWnd">Resources/qss/MainWnd.qss</file>
    </qresource>
</RCC>
```
* 加载qss资源。  
我们可以在进程入口针对整个qt应用设置stylesheet，全部的控件样式都放在一个qss文件中。  
我们也可以针对单个窗口设置qss样式，毕竟对于一个大型项目，往往是分模块、多窗口实现的，每个窗口对qss文件，达到解耦的目的。  
```c++
// 进程入口设置
QApplication a(argc, argv);
a.setStyleSheet(":/qss/MainWnd");

// 针对单个窗口设置
QMainWindow* pWnd;
pWnd->setStyleSheet(":/qss/MainWnd");

// 针对单个控件设置
QWidget* pWidget;
pWidget->setStyleSheet(":/qss/MyPage");
```
需要注意的是，qt的stylesheet是覆盖机制，对于某个控件，最后一次对控件的样式设置才会生效。这也是为什么，我们可以解耦窗口qss样式的原因。  
同时，我们也要注意，一旦我们在代码中针对某个控件设置了样式，我们就需要一次性设置其全部样式，这里并不存在父子继承关系的组合效果。类似下面的代码，无论我们在qss文件中为Btn设置了多么花哨的样式，一旦后面的`pBtn->setStyleSheet("color:red");`被执行，这个btn将只会有color red这一个效果，其他样式都丢弃了。
```c++
// 针对单个窗口设置
QMainWindow* pWnd;
pWnd->setStyleSheet(":/qss/MainWnd");

// MainWindow下有个btn
QPushButton* pBtn;
pBtn->setStyleSheet("color:red");
```
## qss不生效排查
1. 确认qss资源加载正常：文件路径，文件存在等。
2. 先从最外层的窗口开始检查，等外层qss文件生效以后，再逐个排查内层qss样式。
3. 是否有使用`.ui`文件，`.ui`里面是否有设置样式？`ui`文件里面的样式会覆盖qss文件。需要注意的是，QtCreator里面清空控件样式，不要直接输入空串，这可能直接设置了空样式，找找类似还原的功能。
4. 在代码中，是否有针对单个控件设置了stylesheet？单个控件的stylesheet会覆盖qss文件。因此，可以先屏蔽了代码里面的设置。
5. 此控件是否是QWidget？尤其是new QWidget作为某个内部弹框时，qss可能不生效，此时，可以尝试覆写`paintEvent`事件。

以经验来看，多数是第1点和第3点引起的问题。第5点，按照qt的文档应该是要设置`paintEvent`的，但实际发现不设置也照样生效。  
`paintEvent`例子如下。
```
void NativeHomePageHeader::paintEvent(QPaintEvent* event) {
    QStyleOption opt;
    opt.init(this);

    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
```

## qss文件样式小技巧
### 利用Property微调样式
窗口中多数QLabel都是red颜色，只有个别QLable要是rgb调整，可以对控件增加Property属性。  

```c++
// MyPage.h
class MyPage : public QWidget {
    Q_OBJECT
    // ...
};

// MyPage.cpp
pQLable->setProperty("qcolor", "blue");
```
这里还有一个小细节，qss文件里面可以直接使用class名称MyPage，这样会自动关联到MyPage，就不必额外再给MyPage设置ObjectName了。
```
MyPage QLabel {
    color: red;
}

MyPage QLabel[qfont="CESI"] {
    font: 14pt "CESI黑体-GB18030";
}

MyPage QLabel[qcolor="blue"] {
    color: rgb(0, 85, 255);
}
```
### QPushButton增加小icon
```
#LoginButton[qStatus="login"] {
    qproperty-icon: url(:/Img/login_normal);
    qproperty-iconSize: 20px;
}
#LoginButton[qStatus="logout"] {
    qproperty-icon: url(:/Img/logout_normal);
    qproperty-iconSize: 20px;
}
```
