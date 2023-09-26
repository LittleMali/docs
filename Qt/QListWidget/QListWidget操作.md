# QListWidget

不建议使用吧。还是用QListView好了。

注意listWidget释放资源的时候，要显示delete。

## reset
```c++
ui->listWidget->clear();

auto listData = m_pItemModel->GetModelList();
for (auto i = 0; i < listData.size(); ++i) {
    auto* logItem = new CustomWidget(this);
    logItem->SetData(listData.at(i));
    connect(logItem, SIGNAL(ListItemCheckStateChanged(const ShowInfo&)), this,
            SLOT(OnListItemCheckStateChanged(const ShowInfo&)));

    QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
    item->setSizeHint(logItem->size());
    ui->listWidget->setItemWidget(item, logItem);
}
```

## add
```c++
 bool find = false;
for (int i = 0; i < ui->listWidget->count(); ++i) {
    QListWidgetItem* item = ui->listWidget->item(i);
    QWidget* itemWidget = ui->listWidget->itemWidget(item);
    CustomWidget* myItem = qobject_cast<CustomWidget*>(itemWidget);
    if (myItem) {
        auto data = myItem->GetData();
        if (data.IsSame(info)) {
            info.checked = data.checked;  // keep checkbox
            myItem->SetData(info);
            find = true;
        }
    }
}

if (!find) {
    auto* customItem = new CustomWidget(this);
    customItem->SetData(info);
    connect(customItem, SIGNAL(ListItemCheckStateChanged(const ShowInfo&)), this,
            SLOT(OnListItemCheckStateChanged(const ShowInfo&)));

    QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
    item->setSizeHint(customItem->size());
    ui->listWidget->insertItem(0, item);
    ui->listWidget->setItemWidget(item, customItem);
}
```

## remove
```c++
bool find = false;
int index = 0;
for (int i = 0; i < ui->listWidget->count(); ++i) {
    QListWidgetItem* item = ui->listWidget->item(i);
    QWidget* itemWidget = ui->listWidget->itemWidget(item);
    CustomWidget* myItem = qobject_cast<CustomWidget*>(itemWidget);
    if (myItem) {
        auto data = myItem->GetData();
        if (data.IsSame(info)) {
            index = i;
            find = true;
            break;
        }
    }
}

if (find) {
    QListWidgetItem* item = ui->listWidget->takeItem(index);
    if (item) {
        QWidget* itemWidget = ui->listWidget->itemWidget(item);
        CustomWidget* myItem = qobject_cast<CustomWidget*>(itemWidget);
        if (myItem) {
            delete myItem;
        }
        delete item;
    }
}
```