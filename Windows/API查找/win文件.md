# windows文件操作

## 遍历文件
```
FindFirstFile
FindNextFile
FindClose
```
1. 要注意遍历的路径，路径后面要追加 \\*，比如，想遍历C盘test下的所有文件。
```
std::string strDir = "C:\\test";
strDir += "\\*";
WIN32_FIND_DATAA data;
HANDLE hFind = FindFirstFile(strDir.c_str(), &data);
```
2. 文件属性判断 
```
bool bIsDir = data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
```

## ini文件
ini文件，线程安全的，用起来比较方便。
```
WritePrivateProfileString
```