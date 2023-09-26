#pragma once

#include "listdelegate.h"
#include "listmodel.h"
#include "listitem/listitem.h"
#include "page.h"
#include "ui_page.h"

class Page : public Page
{
    using this_class = Page;

    Q_OBJECT
public:
    explicit Page(QWidget *parent = 0);
    ~Page();

    virtual void showEvent(QShowEvent *event) override;

private slots:
    void OnCheckStateChanged();

private:
    void OnAddItem(txcore::txmvvm::Variant *params);

private:
    Ui::Page *ui = Q_NULLPTR;

    ListModel *m_pItemModel = nullptr;
    ListDelegate *m_pDelegate = nullptr;
};
