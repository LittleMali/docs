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