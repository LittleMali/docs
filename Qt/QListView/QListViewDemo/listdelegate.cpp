#include "listdelegate.h"

#include <QApplication>
#include <QCheckBox>
#include <QMouseEvent>
#include <QPainter>
#include <QTextEdit>
#include <QToolTip>

#include "listmodel.h"
#include "listitem/listitem.h"

ListDelegate::ListDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
    m_listItem = new ListItem(static_cast<QWidget *>(parent));
    m_listItem->hide();
}

ListDelegate::~ListDelegate() {}

QSize ListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // 我们是固定大小的，可以直接return size()
    // 如果是动态大小，那么要先把 listItem update一下，再获取size。
    return m_listItem->size();
}

void ListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
    // QStyledItemDelegate::paint(painter, option, index);

    if (!index.isValid())
    {
        return;
    }

    painter->save();
    QWidget *mypainter = RenderItemWidget(index);
    painter->translate(option.rect.topLeft());
    if (mypainter)
    {
        mypainter->render(painter, QPoint(0, 0), QRegion(), QWidget::DrawChildren);
    }

    painter->restore();
}

bool ListDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,
                               const QModelIndex &index)
{
    do
    {
        if (event->type() != QEvent::MouseButtonPress)
        {
            break;
        }

        if (!index.isValid())
        {
            break;
        }

        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        QRect checkBoxRect = QRect(option.rect.left(), option.rect.top(), option.rect.width(), option.rect.height());

        if (checkBoxRect.contains(mouseEvent->pos()))
        {
            // checkbox click

            if (!m_enableCheckbox)
            {
                QFont font("Noto Sans CJK SC", 9, QFont::Normal);
                font.setPixelSize(12);
                QToolTip::setFont(font);
                QToolTip::showText(QCursor::pos(), "被管理员禁止");
                break;
            }

            QVariant var = index.data(Qt::UserRole + 1);
            if (!var.canConvert<ShowInfo>())
            {
                break;
            }

            ShowInfo info = var.value<ShowInfo>();

            info.checked = !(info.checked);

            QVariant newData;
            newData.setValue(info);
            model->setData(index, newData, Qt::UserRole + 1);

            emit CheckStateChanged();
        }

    } while (false);

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

QWidget *ListDelegate::RenderItemWidget(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return nullptr;
    }

    QVariant var = index.data(Qt::UserRole + 1);
    if (!var.canConvert<ShowInfo>())
    {
        return nullptr;
    }

    ShowInfo info = var.value<ShowInfo>();
    m_listItem->UpdateData(info, m_enableCheckbox);

    return m_listItem;
}

void ListDelegate::UpdateModifyStatus(bool enabled) { m_enableCheckbox = enabled; }