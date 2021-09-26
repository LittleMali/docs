# 系统功能
## 剪切板
``` c++
HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, szBuffer, dwBufferSize + 1);
memcpy(GlobalLock(hMem), szBuffer, dwBufferSize);
GlobalUnlock(hMem);

OpenClipboard(0);

EmptyClipboard();
SetClipboardData(CF_TEXT, hMem);

CloseClipboard();
```