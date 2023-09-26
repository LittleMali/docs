#pragma once

#include <QAbstractListModel>
#include <QList>

class ListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    ListModel(QObject *parent);
    ~ListModel();

    void UpdateModelItem(const ShowInfo &item);

    QList<ShowInfo> GetModelList();

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

private:
    QList<ShowInfo> m_oList;
};
