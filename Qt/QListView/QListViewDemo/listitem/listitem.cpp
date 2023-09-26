#include "listitem.h"

#include <QPixmap>

ListItem::ListItem(QWidget *parent) : QWidget(parent), ui(new Ui::ListItem)
{
    ui->setupUi(this);
    // 设置响应逻辑

    setStyleSheet("background: none");
    setAttribute(Qt::WA_TranslucentBackground);
}

ListItem::~ListItem() { delete ui; }

void ListItem::UpdateData(const ShowInfo &data, bool enableCheckbox)
{
    if (enableCheckbox)
    {
        this->setEnabled(true);
        ui->checkBox->setChecked(data.checked);
    }
    else
    {
        this->setEnabled(false);
        ui->checkBox->setChecked(false);
    }

    {
        QFontMetrics metrics(ui->fileName->font());
        QString elidedText = metrics.elidedText(data.FileName.c_str(), Qt::ElideRight, 350);
        ui->fileName->setText(elidedText);
    }
}
