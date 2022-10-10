# Socket

## tips
* 异步socket设置
```c
unsigned long ul = 1;
int iRet = ioctlsocket(sock, FIONBIO, &ul); 
```