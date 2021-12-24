<!--
 * @Author: your name
 * @Date: 2021-12-23 11:12:24
 * @LastEditTime: 2021-12-24 09:55:15
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \docs\Qt\Qt控件入门.md
-->
# 控件入门
本文档记录一下如何添加控件并调整控件位置

|-------------------|
|  title            |
|  content          |
|       sure cancel |
|-------------------|

下面的代码是在mainwindow中直接添加控件。
对于mainwindow而言，我们可以先创建一个主Widget作为外层控件，再在主Widget里面添加子控件。
1. new pMainWidgt，最外层控件，也可以理解为“父窗口”，我们之后都是在这个“父窗口”上添加子控件。
   设置属性使用了 *#MainWidget {border:1px solid red;}* 语法，这种写法类似css，是将此样式限定在 名字为MainWidget 的控件上，避免影响到子控件。这是因为样式是继承的，子控件自动继承父控件的样式，如果不加 #MainWidget 限定，则内部所有的子控件都会有边框。
2. new title 和 content控件，并设置属性。
3. new btns，我们添加了2个btn，这两个btn我们希望并排排列，所以，在btns的外面创建了一个QHBoxLayout水平布局，并且把btns添加到水平布局中。
   对于布局，我们最好清除掉Qt的默认设置，setSpacing 和 setContentsMargins 清理掉默认间距，其次，显示设置对齐方式，因为默认的居中不一定符合我们的需求。
   注意，这里的pBtnLayout是没有传父控件进去的，因为这也是一个子布局，会被后面的 主布局 所添加。
   如果有多处地方设置了QHBoxLayout(parent)，那么一个parent有了多个布局，Qt会提示warning。
4. new 主布局pMainLayout，把各个控件和布局添加进来。

在实际开发中，我们会按照设计稿来调整控件的位置，几个px的对齐。我们可以通过Layout来添加控件，在Layout中设置对齐方式，调整spacing或者设置margin来微调子控件的位置，最终将多个Layout添加到主布局中，以此来组装整个ui界面。通过布局微调位置，比直接设置控件pos要好很多。

所以，整个的效果如下所示。
![20211223120947](https://raw.githubusercontent.com/LittleMali/docs/master/mdPics/20211223120947.png)   

```c++
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget* pMainWidgt = new QWidget(this);
    pMainWidgt->setFixedSize(400, 200);
    pMainWidgt->setObjectName("MainWidget");
    pMainWidgt->setStyleSheet("#MainWidget {border:1px solid red;}");

    QLabel* pTitle = new QLabel();
    pTitle->setStyleSheet("font-size:20px; color:#000000");
    pTitle->setText("标题");

    // content text
    QLabel* pContent = new QLabel();
    pContent->setStyleSheet("font-size:12px; color:#000000");
    pContent->setText("我是很长的文案");

    // bottom two btns
    QPushButton* pSureBtn = new QPushButton();
    pSureBtn->setFixedSize(50, 25);
    pSureBtn->setText("确定");

    QPushButton* pCancelBtn = new QPushButton();
    pCancelBtn->setFixedSize(50, 25);
    pCancelBtn->setText("取消");

    QHBoxLayout* pBtnLayout = new QHBoxLayout();    // no parent
    pBtnLayout->setAlignment(Qt::AlignRight);
    pBtnLayout->setSpacing(0);
    pBtnLayout->setContentsMargins(0, 0, 0, 0);
    pBtnLayout->addWidget(pSureBtn, 0, Qt::AlignRight);
    pBtnLayout->addSpacing(100);
    pBtnLayout->addWidget(pCancelBtn, 0, Qt::AlignRight);

    // main layout
    QVBoxLayout* pMainLayout = new QVBoxLayout(pMainWidgt); // has parent
    pMainLayout->setAlignment(Qt::AlignTop);
    pMainLayout->setSpacing(0);
    pMainLayout->setContentsMargins(0, 0, 0, 0);
    pMainLayout->addSpacing(20);
    pMainLayout->addWidget(pTitle, 0, Qt::AlignLeft);
    pMainLayout->addSpacing(30);
    pMainLayout->addWidget(pContent, 0, Qt::AlignLeft);
    pMainLayout->addLayout(pBtnLayout);
}
```
