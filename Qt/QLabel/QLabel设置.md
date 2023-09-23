# QLabel设置

## 样式
### 文本过长时自动省略
```c++
QString fullText = "This is a very long text that will not fit into the label";
QLabel *label = new QLabel;

QFontMetrics metrics(label->font());
QString elidedText = metrics.elidedText(fullText, Qt::ElideRight, label->width());

label->setText(elidedText);
```