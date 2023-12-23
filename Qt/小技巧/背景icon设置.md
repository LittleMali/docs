# Qt背景设置

我们的背景是一个小ico和一个文本，如下。
``` c++
// tab style example
/*
|------------------------|
|                        |
|         |-----|        |
|         | ico |  文本  |
|         |-----|        |
|     |
| --- |
*/

几个核心的属性是：
background-position: center; // 背景居中。
text-align: center; // 文本居中
padding-left: 46px; // 文本左侧间距46px

所以，这是一个取巧的实现，不是 [ico+文本] 整体居中，而是ico居中，文本稍微靠右。
没办法，Qt的qss不是完全等价于css，css是可以直接设置background-position的位置。
```

qss样式如下。
```
QPushButton {
    background-image: url(:/tabbtn/normal);
    background-repeat: no-repeat;
    background-color: #F2F7FF;
    background-position: center;
    border: none;
    color: #888;
    font-size: 12px;
    text-align: center;
    padding-left: 46px;
}

QPushButton:hover {
    background-image: url(:/tabbtn/hover);
    background-repeat: no-repeat;
    background-color: #F2F7FF;
    background-position: center;
    border: none;
    color: #888;
    font-size: 12px;
    text-align: center;
    padding-left: 46px;
}

QPushButton:checked {
    background-image: url(:/tabbtn/pressed);
    background-repeat: no-repeat;
    background-color: #FFFFFF;
    background-position: center;
    border: none;
    color: #005AFF;
    font-size: 12px;
    text-align: center;
    padding-left: 46px;
}
```