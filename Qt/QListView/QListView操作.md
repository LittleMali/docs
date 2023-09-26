# QListView

## 介绍
QListView是基于view-model-delegate方式来维护控件的。
* view：控件视图，控制整个控件。
* model：数据，所有list item的数据都由model来维护。
* delegate：代理试图，对单个list item的绘制和操作，由delegate实现。

当model有数据变化时，无论是刷新单个item data还是新增item data，都会触发delegate会paint list item ui，QListView只会重绘对应的list item，所以绘制的效率较高。  
QListView实际是个虚拟list，它创建的一个个item都是虚拟item，单个item其实只是在一个widget上paint一个背景，它并不是一系列xxx控件组成的自定义ui。虚拟item更加轻量，所以创建的效率很高。  
所以，QListView是基于model-delegate来维护list控件的，这是它性能好的原因。相对的QListWidget是基于item来维护list的，在QListWidget中，每个item都是一个完整的自定义ui，所以它的创建成本更高，同时，数据刷新效率也低。  

如果list的数据量**大于100**，那么就直接选择QListView，不要使用QListWidget。QListWidget一次性新增1000+个item时，耗时要4s多。这个性能无法接受。

## 用法
### view
参考QListViewDemo，在父控件Page中创建model和delegate，并设置关联关系。
```c++
// Page.cpp
Page::Page(QWidget *parent) : Page(parent), ui(new Ui::Page)
{
    ui->setupUi(this);

    m_pItemModel = new PageListModel(this);
    m_pDelegate = new ListDelegate(this);
    ui->listView->setModel(m_pItemModel);
    ui->listView->setItemDelegate(m_pDelegate);

    connect(m_pDelegate, SIGNAL(CheckStateChanged()), this, SLOT(OnCheckStateChanged()));
}
```
### model
依据业务需求，在model中提供数据增删查改的接口，例如，有数据来的时候就Update一次。  

我们还需要实现QAbstractListModel的几个接口，delegate在绘制ui时，会自动从model里面获取数据。  
* rowCount：多少行。
* data：获取某个类型的数据。我们是自定义的数据结构，所以，使用的是`Qt::UserRole + 1`。
* setData：更新数据，如果在delegate里面更新了数据，需要通过这个接口刷新model。
```c++
// listmodel.h
 void UpdateModelItem(const ShowInfo &item);

virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
```
### delegate
delegate主要是绘制单个list item，并响应item的点击事件。
* paint：绘制背景。  
  可以看到，我们的Delegate只new了一个widget出来，我们在paint里面仅仅是刷新了widget，然后render到背景上。从这里，就能明白，什么是虚拟list了。我们只创建了一个widget，所有的item都是用这个widget去render了一下，仅仅是render。
* editorEvent：点击事件。  
  因为我们是虚拟list，所以，仅仅拿到点击事件，这个事件是没办法跟ListItem绑定起来的，我们没有办法在ListItem里面去操作这些事件。所以，我们只能通过鼠标点击的位置来计算一下，是不是点击了checkbox。我们的代码里面取巧了，整个item的点击都视为checkbox点击了。
```c++
// listdelegate.h
virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
 virtual bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,
                             const QModelIndex &index) override;
```

## 和QListWidget的区别
最大的区别：view是虚拟list！！！  

xxxView只是一个虚拟list，仅仅是render了一个背景，并没有实际的自定义控件绑定到item上。  

xxxWidget是有实际自定义控件绑定到item上的，每一个item都有一个new CustomWidget控件存在，所以，只有调用了QListWidget::setItemWidget(item, customWidget)才能真正显示控件，这也就是为什么xxxWidget性能低下的原因。

