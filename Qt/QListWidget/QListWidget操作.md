
## reset
```c++
ui->listWidget->clear();

    auto listData = m_pItemModel->GetModelList();
    for (auto i = 0; i < listData.size(); ++i) {
        auto* logItem = new VirusScanLogListItem(this);
        logItem->SetData(listData.at(i));
        connect(logItem, SIGNAL(ListItemCheckStateChanged(const LogShowInfo&)), this,
                SLOT(OnListItemCheckStateChanged(const LogShowInfo&)));

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
        VirusScanLogListItem* myItem = qobject_cast<VirusScanLogListItem*>(itemWidget);
        if (myItem) {
            auto data = myItem->GetData();
            if (data.IsSame(logInfoItem)) {
                logInfoItem.checked = data.checked;  // keep checkbox
                myItem->SetData(logInfoItem);
                find = true;
            }
        }
    }

    if (!find) {
        auto* denyItem = new VirusScanLogListItem(this);
        denyItem->SetData(logInfoItem);
        connect(denyItem, SIGNAL(ListItemCheckStateChanged(const LogShowInfo&)), this,
                SLOT(OnListItemCheckStateChanged(const LogShowInfo&)));

        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        item->setSizeHint(denyItem->size());
        ui->listWidget->insertItem(0, item);
        ui->listWidget->setItemWidget(item, denyItem);
    }
```

## remove
```c++
bool find = false;
    int index = 0;
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        QListWidgetItem* item = ui->listWidget->item(i);
        QWidget* itemWidget = ui->listWidget->itemWidget(item);
        VirusScanLogListItem* myItem = qobject_cast<VirusScanLogListItem*>(itemWidget);
        if (myItem) {
            auto data = myItem->GetData();
            if (data.IsSame(logInfoItem)) {
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
            VirusScanLogListItem* myItem = qobject_cast<VirusScanLogListItem*>(itemWidget);
            if (myItem) {
                delete myItem;
            }
            delete item;
        }
    }
```