#pragma once

#include <QStyledItemDelegate>

#include "listitem/listitem.h"

class ListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ListDelegate(QObject *parent);
    ~ListDelegate();

    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void UpdateModifyStatus(bool enabled);
signals:
    void CheckStateChanged();

protected:
    virtual bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,
                             const QModelIndex &index) override;

    QWidget *RenderItemWidget(const QModelIndex &index) const;

private:
    ListItem *m_listItem = nullptr;
    bool m_enableCheckbox = true;
};