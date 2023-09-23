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
}

QListWidget::item {
    background-color: #F9F9FB;
}

QListWidget::item:alternate {
    background-color: #FFF;
}

QListWidget::item::selected {
    background: none;
}
```