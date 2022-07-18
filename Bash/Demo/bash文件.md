# bash文件

## 文件是否存在
```bash
#!/bin/bash

if [[ ! -f "$TEST_FILE" ]]; then
    echo "test path is not exits"
    exit 1
fi

if exists aa.txt (
    if not exists bb.exe (
        echo "bb is NOT exists"
    )
)

if exists xx.txt (
    del xx.txt
)
```