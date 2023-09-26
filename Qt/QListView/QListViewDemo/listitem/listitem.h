#pragma once

#include <QWidget>

#include "ui_listitem.h"

class ListItem : public QWidget
{
    Q_OBJECT

public:
    explicit ListItem(QWidget *parent = nullptr);
    ~ListItem();

    void UpdateData(const ShowInfo &data, bool enableCheckbox);

private:
    Ui::ListItem *ui = Q_NULLPTR;
};
