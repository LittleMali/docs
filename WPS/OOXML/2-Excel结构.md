# Excel结构

在Office Open XML（OOXML）中，工作簿（Workbook）是Excel文档的顶级结构，它包含了文档中的所有工作表（Worksheets）以及与整个文档相关的设置和属性。工作簿的数据存储在名为xl/workbook.xml的文件中。

Excel文件存储目录结构：
```
OOXML文档
│
├─ [Content_Types].xml
│
├─ _rels
│   └─ .rels
│
├─ docProps
│   ├─ core.xml
│   └─ app.xml
│
└─ xl (对于Excel文档，类似的结构适用于Word和PowerPoint)
    ├─ workbook.xml
    │
    ├─ _rels
    │   └─ workbook.xml.rels
    │
    ├─ worksheets
    │   ├─ sheet1.xml
    │   ├─ sheet2.xml
    │   └─ ...
    │
    ├─ styles.xml
    │
    ├─ sharedStrings.xml
    │
    ├─ theme
    │   └─ theme1.xml
    │
    └─ media
        ├─ image1.png
        ├─ image2.jpg
        └─ ...
```

## workbook属性
以下是工作簿（Workbook）中的一些常用元素和属性：
