# Qt勾选框
QCheckBox的使用

```c++
QStyleOptionButton checkBoxStyle;
checkBoxStyle.state = infoItem->m_check ? QStyle::State_On : QStyle::State_Off;
checkBoxStyle.state |= QStyle::State_Enabled;
checkBoxStyle.iconSize = QSize(15, 15);
checkBoxStyle.rect = QRect(option.rect.left() + 15, option.rect.top() + 5, 15, 15);

QCheckBox checkBox;
QApplication::style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &checkBoxStyle, painter, &checkBox);
```

PyQt5的QLabel，使用 .setStyleSheet('text-align: center;') 尝试文字居中，居然无效!!!

可使用 .setAlignment(Qt.AlignCenter) 文字上下左右居中。