# 函数记录
## 读写文件
### 读文件
``` c++
#include <fstream>
#include <iostream>
#include <sstream>

std::string strFile = "a.txt";
std::ifstream ifs(strFile, std::ios::in | std::ios::binary);
std::stringstream strBuffer;
std::string strContent;

if (ifs.is_open()) {
    strBuffer << ifs.rdbuf();
    strContent.assign(strBuffer.str());

    ifs.close();
}
```

### 写文件
```c++
ofstream of;
of.open(strFile.c_str(), ios::out | ios::trunc);
if (of.is_open())
{
    of << strContent;
    of.close();
}
```
## printf  
`printf("%zu", vector.size())`