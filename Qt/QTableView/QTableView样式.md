Qt中样式表对选中颜色和悬停颜色是有优先级的，根据对操作系统默认样式的观察，当处于选中状态+悬停状态的时候，默认取悬停状态，也就是鼠标移动到选中的列表item上，颜色取悬停状态颜色。而Qt中如果两种颜色都设置了，根据设置的顺序来，取最后的为准，如果最后设置的选中状态颜色，则当item处于选中状态+悬停状态的时候，取选中状态颜色而不是悬停状态颜色，切记！


```c++
//下面这样设置则当鼠标停留在选中的item上时背景颜色=#00FF00
QTableView::item:selected{background:#FF0000;}
QTableView::item:hover{background:#00FF00;}

//下面这样设置则当鼠标停留在选中的item上时背景颜色=#FF0000
QTableView::item:hover{background:#00FF00;}
QTableView::item:selected{background:#FF0000;}

//左上角样式很容易忽略
QTableCornerButton:section{background:#FF0000;}
```


当我们对QTableView/QTreeView/QTableWidget/QTreeWidget某行选中后，会发现某些单元格设置的前景色被覆盖了，比如设置的红色，一旦选中就变成了白色，这肯定不是我们想要的，需要用自定义委托将其去掉。

```c++
class ItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit ItemDelegate(QObject *parent = 0);

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#include "itemdelegate.h"

ItemDelegate::ItemDelegate(QObject *parent) : QItemDelegate(parent)
{

}

void ItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem option2 = option;
    QColor color = index.data(Qt::ForegroundRole).value<QColor>();
    if (color.isValid() && color != option.palette.color(QPalette::WindowText)) {
        option2.palette.setColor(QPalette::HighlightedText, color);
    }

    QItemDelegate::paint(painter, option2, index);
}

//对所有单元格设置该委托
ui->tableWidget->setItemDelegate(new ItemDelegate);
```