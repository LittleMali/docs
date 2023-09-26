#include "page.h"

#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QScrollBar>
#include <QVBoxLayout>

Page::Page(QWidget *parent) : Page(parent), ui(new Ui::Page)
{
    ui->setupUi(this);

    m_pItemModel = new PageListModel(this);
    m_pDelegate = new ListDelegate(this);
    ui->listView->setModel(m_pItemModel);
    ui->listView->setItemDelegate(m_pDelegate);

    connect(m_pDelegate, SIGNAL(CheckStateChanged()), this, SLOT(OnCheckStateChanged()));
}

Page::~Page()
{
    delete ui;
}

void Page::OnAddItem()
{
    if (!m_pItemModel)
    {
        return;
    }

    ShowInfo infoItem;
    m_pItemModel->UpdateModelItem(infoItem);
}
