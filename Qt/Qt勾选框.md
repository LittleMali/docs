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


## QCheckBox设置图片
关键词： indicator 。
QCheckBox默认是带一个正方形的勾选框，前缀ico的样式由indicator设置。

QRadioButton也是类似的。
```css
QCheckBox {
    font-size: 12px;
    font-family: Noto Sans CJK SC;
    color: #444;
    background-color: none;
}

QCheckBox:disabled {
    font-size: 12px;
    font-family: Noto Sans CJK SC;
    color: #BBB;
    background-color: none;
}

QCheckBox::indicator {
    width: 14px;
    height: 14px;
    background: url(:/icon/checkbox_uncheck);
}

QCheckBox::indicator:checked {
    width: 14px;
    height: 14px;
    background: url(:/icon/checkbox_checked);
}

/*部分选中*/
QCheckBox::indicator:indeterminate {
    width: 14px;
    height: 14px;
    background: url(:/icon/checkbox_partial);
}

QCheckBox::indicator:disabled {
    width: 14px;
    height: 14px;
    background: url(:/icon/checkbox_disable_uncheck);
}

QCheckBox::indicator:checked:disabled {
    width: 14px;
    height: 14px;
    opacity: 0.2;
    background: url(:/icon/checkbox_disable_checked);
}

```