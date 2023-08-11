# Qt样式杂谈

1. QWidget的背景和边框。  
   QWdiget的背景和边框有时候设置了不生效，不好排查时，不要使用QWidget，改为QFrame。因为QWidget太底层了，一些UI样式它确实不支持。