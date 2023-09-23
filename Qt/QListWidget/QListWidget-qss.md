```css
QListWidget {
    color: #888;
    font-family: Noto Sans CJK SC;
    font-size: 12px;
    font-style: normal;
    font-weight: 400;
    line-height: 20px;
    border: none;
    background: #FFF;
    outline: 0px; // 选中时的边框
}

QListWidget::item {
    background-color: #F9F9FB;
}

QListWidget::item:alternate {
    background-color: #FFF;
}

// 在ui文件中设置不允许编辑，不允许选中，下面这些效果就不要再设置了，设置了有反作用。
QListWidget::item::selected {
    background: blue;
}

QListWidget::item::pressed {
    background: red;
}

QListWidget::item:hover {
    background: transparent;
}
```