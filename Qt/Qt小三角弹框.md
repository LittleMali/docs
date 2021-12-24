<!--
 * @Author: your name
 * @Date: 2021-12-24 09:58:36
 * @LastEditTime: 2021-12-24 15:12:27
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \docs\Qt\Qt小三角弹框.md
-->

# 小三角弹框
我们想实现这么一个弹框，点击一个控件时弹出详细信息的三角弹框，当再点击其他位置时三角弹框自动关闭。

![20211224111440](https://raw.githubusercontent.com/LittleMali/docs/master/mdPics/20211224111440.png)

关键技术点如下。
1. 加阴影
   加阴影是利用Qt的*QGraphicsDropShadowEffect*功能，此设置会自动添加阴影，但是，此阴影只能添加到控件上，无法自动添加到窗口上，所以，这里要配合*paintEvent*来实现。

   ![20211224120728](https://raw.githubusercontent.com/LittleMali/docs/master/mdPics/20211224120728.png)
2. 画边框和三角
   * 画边框是重写了Qt的 *paintEvent* 函数，使用QPainter自绘边框和三角形。
   painter.setBrush设置背景，此背景会填充窗口的背景，窗口里面的控件背景是不受影响的。这一点this->setStyleSheet设置的背景有差异，窗口如果设置了setStyleSheet，内部子控件会默认继承窗口的样式，从而影响到子控件的背景。
   * 画方框和三角，重点在于线条坐标的计算，所以，这里要区分清楚几个宽度。
  
    ![20211224142649](https://raw.githubusercontent.com/LittleMali/docs/master/mdPics/20211224142649.png)
    
    WIDGET_WIDTH：黑框范围，包括阴影在内的size。
    
    SHADOW_WIDTH：黑框和紫框之间的间距，当然，矩形的上部还要考虑三角的高度。
    
    CONTENT_WIDTH：紫框范围，通常来说，设计稿中我们看到的就是这个区域。
    
    CONTENT_PADDING：紫框和蓝框之间的间距，我们的控件不可能贴着紫框摆放，两者之间的间距就是padding。
    
    弄清楚一个阴影窗口由那几部分组成，我们就可以知道怎么画边框线条了。

    ```c++
    // 箭头朝上的小三角，正方形边框的画法
    drawPath.addRoundedRect(
        QRect(SHADOW_WIDTH,  // 距离窗口左侧，要空出阴影的宽度来
            SHADOW_WIDTH + TRIANGLE_HEIGHT, // 距离窗口的顶部，要空出阴影和小三角的高度
            width() - SHADOW_WIDTH * 2,
            height() - TRIANGLE_HEIGHT - SHADOW_WIDTH * 2),
        ...
    )
    
    // 箭头朝上的小三角，小三角边框的画法
    int startX = 20 + SHADOW_WIDTH; // 设计稿中，三角距离边框的位置，也就是距离content border的距离。
    trianglePolygon << QPoint(startX, SHADOW_WIDTH + TRIANGLE_HEIGHT);  // 三角形左点的坐标
    trianglePolygon << QPoint(startX + TRIANGLE_WIDTH / 2, SHADOW_WIDTH); // 三角形顶点的坐标
    trianglePolygon << QPoint(startX + TRIANGLE_WIDTH, SHADOW_WIDTH + TRIANGLE_HEIGHT); // 三角形右点的坐标
    ```
3. 失去焦点消失  
   点击“确定”btn，三角弹框出现。在窗口其他位置点击一下，弹框消失。这种实现类似PopUp wnd，所以，给窗口设置Popup属性即可：setWindowFlags(Qt::FramelessWindowHint | Qt::Popup)。如果我们不加popup，那么弹框将成为一个独立的窗口，我们在系统菜单栏可以看到两个窗口。
   
   同时，想要实现阴影效果，还要设置*this->setAttribute(Qt::WA_TranslucentBackground);*，这个属性就没有仔细查了，网上copy过来直接用。
4. 自动消失的另外一个做法是安装event filter。
   ```c++
   // init时安装filter
   QWidget::installEventFilter(this);
   // uninit时卸载filter
   QWidget::removeEventFilter(this);
   // 重写filter过滤器
   // 这个函数也比较好懂，如果当前控件是自身，并且事件是失去焦点，则关闭弹框。
   bool TriangleWidget::eventFilter(QObject *obj, QEvent *event)
    {
        bool bRet = false;
        do
        {
            if(obj != this)
                break;

            if(QEvent::WindowDeactivate != event->type())
                break;

            this->close();

            bRet = true;
        }while(false);

        return bRet;
    }
   ```
   不过，安装filter的代码在本例子中并没有跑通，filter跟窗口的属性有关系。既然决定安装filter，我们就不需要Qt::Popup了，但是，为了避免出现两个窗口，我们尝试设置了Qt::Tool，很遗憾没有达到预期，虽然event事件是收到了，但是弹框没有消失。这里估计还是哪个细节没写好，可能跟TriangleWidget的父窗口有关系，父窗口parent和Qt::Tool估计有冲突。
5. 弹框显示的位置
   
   弹框怎么显示：QWidget::show。
   
   弹框显示到哪里：全局坐标定位，pTriWidget->SetPos(x, y)传入的是global坐标，也就说，是相对显示屏的坐标，以显示屏左上角作为原点的坐标。

```c++
// TriangleWidget.h
#ifndef TRIANGLEWIDGET_H
#define TRIANGLEWIDGET_H

#include <QWidget>

class TriangleWidget : public QWidget {
    Q_OBJECT

public:
    explicit TriangleWidget(QWidget* pParent = nullptr);
    virtual ~TriangleWidget();

    void SetPos(int nPosX, int nPosY);
private:
    void Init();
    void UnInit();

protected:
    void paintEvent(QPaintEvent* pEvent) override;
};

#endif // TRIANGLEWIDGET_H
```


```c++
// TriangleWidget.cpp
#include "TriangleWidget.h"
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QEvent>
#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>

namespace {
// pixel
const int TRIANGLE_WIDTH = 12;
const int TRIANGLE_HEIGHT = 12;

const int BORDER_RADIUS = 5;
const int SHADOW_WIDTH = 15;
const int CONTENT_PADDING = 18; // inner ctrl to border

const int CONTENT_WIDTH = 250;
const int CONTENT_HEIGHT = 120;

const int WIDGET_WIDTH = (CONTENT_WIDTH + SHADOW_WIDTH * 2);
const int WIDGET_HEIGHT = (CONTENT_HEIGHT + SHADOW_WIDTH * 2 + TRIANGLE_HEIGHT);
}

TriangleWidget::TriangleWidget(QWidget* pParent) : QWidget(pParent) {
    //setStyleSheet("background-color: #FFFFFF"); // background is white !!!
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup); // popup wnd !!!
    this->setAttribute(Qt::WA_TranslucentBackground); // wnd need this attribute

    auto shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setOffset(0, 0);
    shadowEffect->setColor(Qt::red);
    shadowEffect->setBlurRadius(SHADOW_WIDTH);
    this->setGraphicsEffect(shadowEffect); // add border shadow

    setFixedSize(WIDGET_WIDTH, WIDGET_HEIGHT);

    Init();
}

TriangleWidget::~TriangleWidget() {
    UnInit();   // nothing todo
}

void TriangleWidget::SetPos(int nPosX, int nPosY) {
    int x = nPosX - (WIDGET_WIDTH) / 2;
    int y = nPosY - WIDGET_HEIGHT + SHADOW_WIDTH * 2;

    this->move(x, y);   // 自己再微调位置，示例代码先忽略了。
    this->move(nPosX, nPosY); 
}

void TriangleWidget::Init() {
    QVBoxLayout* pMainLayout = new QVBoxLayout(this);   // main layout
    pMainLayout->setAlignment(Qt::AlignTop);
    pMainLayout->setSpacing(0);

    // title
    QLabel* pTitle = new QLabel(this);
    pTitle->setText("wnd title");

    // desc
    QLabel* pDesc = new QLabel(this);
    pDesc->setText("this is a triangle widget example");

    pMainLayout->addWidget(pTitle);
    pMainLayout->addWidget(pDesc);

    pMainLayout->setContentsMargins(SHADOW_WIDTH + CONTENT_PADDING, 
                                    SHADOW_WIDTH + CONTENT_PADDING + TRIANGLE_HEIGHT,
                                    SHADOW_WIDTH + CONTENT_PADDING, 
                                    SHADOW_WIDTH + CONTENT_PADDING);
}

void TriangleWidget::UnInit(){

}

void TriangleWidget::paintEvent(QPaintEvent* ) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0xFFFFFF)); // white bkg !!!

    int startX = 20 + SHADOW_WIDTH; // triangle to left border is 20px

    QPolygon trianglePolygon;
    QPainterPath drawPath;

    bool bUp = true;
    if (bUp) {
        trianglePolygon << QPoint(startX, SHADOW_WIDTH + TRIANGLE_HEIGHT);
        trianglePolygon << QPoint(startX + TRIANGLE_WIDTH / 2, SHADOW_WIDTH);
        trianglePolygon << QPoint(startX + TRIANGLE_WIDTH, SHADOW_WIDTH + TRIANGLE_HEIGHT);

        drawPath.addRoundedRect(QRect(SHADOW_WIDTH, SHADOW_WIDTH + TRIANGLE_HEIGHT, width() - SHADOW_WIDTH * 2,
                                      height() - TRIANGLE_HEIGHT - SHADOW_WIDTH * 2),
                                BORDER_RADIUS, BORDER_RADIUS);
    } else {
        trianglePolygon << QPoint(startX, WIDGET_HEIGHT - SHADOW_WIDTH - TRIANGLE_HEIGHT);
        trianglePolygon << QPoint(startX + TRIANGLE_WIDTH / 2, WIDGET_HEIGHT - SHADOW_WIDTH);
        trianglePolygon << QPoint(startX + TRIANGLE_WIDTH, WIDGET_HEIGHT - SHADOW_WIDTH - TRIANGLE_HEIGHT);

        drawPath.addRoundedRect(QRect(SHADOW_WIDTH, SHADOW_WIDTH, width() - SHADOW_WIDTH * 2,
                                      height() - TRIANGLE_HEIGHT - SHADOW_WIDTH * 2),
                                BORDER_RADIUS, BORDER_RADIUS);
    }

    drawPath.addPolygon(trianglePolygon);
    painter.drawPath(drawPath);
}
```

```c++
// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include "TriangleWidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPushButton* pSureBtn = new QPushButton(this);
    pSureBtn->setFixedSize(50, 25);
    pSureBtn->setText("确定");
    connect(pSureBtn, SIGNAL(clicked()), this, SLOT(OnShowTriWidget()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnShowTriWidget()
{
    QPushButton* pBtn = qobject_cast<QPushButton*>(sender());
    QPoint pos = QWidget::mapToGlobal((pBtn->pos()));
    int x = pos.x() + pBtn->width() / 2;
    int y = pos.y() + pBtn->height();

    TriangleWidget* pTriWidget = new TriangleWidget(this); // parent，不设置也可以。
    pTriWidget->SetPos(x, y);
    pTriWidget->show();
}

```