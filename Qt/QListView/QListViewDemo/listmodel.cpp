#include "listmodel.h"

#include <QColor>

ListModel::ListModel(QObject *parent) : QAbstractListModel(parent) {}

ListModel::~ListModel() {}

void ListModel::UpdateModelItem(const ShowInfo &item)
{
    beginResetModel();
    bool find = false;
    for (auto it = m_oList.begin(); it != m_oList.end(); ++it)
    {
        if (it->aa == item.aa && it->bb == item.bb)
        {
            *it = item;
            find = true;
            break;
        }
    }

    if (!find)
    {
        m_oList.insert(0, item);
    }
    endResetModel();
}

QList<ShowInfo> ListModel::GetModelList() { return m_oList; }

int ListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_oList.count();
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{
    QVariant var = QVariant();

    do
    {
        if (!index.isValid())
        {
            break;
        }

        int nRow = index.row();
        if (nRow >= m_oList.size())
        {
            break;
        }

        ShowInfo info = m_oList.at(nRow);

        switch (role)
        {
        case Qt::ForegroundRole:
        {
            var = QColor(Qt::black);
            break;
        }
        case Qt::TextAlignmentRole:
        {
            var = QVariant(Qt::AlignHCenter | Qt::AlignVCenter);
            break;
        }
        case Qt::UserRole + 1:
        {
            var.setValue(info);
            break;
        }
        case Qt::DisplayRole:
        {
            break;
        }
        default:
            break;
        }
    } while (false);

    return var;
}

bool ListModel::setData(const QModelIndex &index, const QVariant &data, int role)
{
    if (!index.isValid())
        return false;

    int nRow = index.row();
    if (nRow < m_oList.size())
    {
        ShowInfo info = data.value<ShowInfo>();
        m_oList[nRow] = info;
    }

    emit dataChanged(index, index);
    return true;
}
