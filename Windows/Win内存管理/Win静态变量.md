# Win静态变量

## win静态变量-1
A.exe  
$~~$|_ Data.dll  
$~~~~~~$|_ static ClassObj  
$~~~~~~~~~~$|_ static char[1000]

B.exe  
$~~$|_ Data.dll  
$~~~~~~$|_ static ClassObj  
$~~~~~~~~~~$|_ static char[1000]

1. Data.dll向外暴露静态变量ClassObj。
2. A.exe和B.exe都加载Data.dll并使用静态变量。
3. 在windows中，Kernel32.dll会被映射到固定的地址，其他dll则是优先尝试映射到首选地址（编译时决定的ImageBase），若该地址被占用，则重新映射，此时，需要重定向（PE结构有存重定向信息）。
4. dll被进程加载以后，被映射到进程的内存空间，每个进程的虚拟内存都是隔离的，因此，A.exe和B.exe虽然加载了同一个dll，但实际操作静态变量ClassObj时使用的是两份内存数据（写时复制机制）。A.exe对dll导出的静态变量的操作，不会影响到B.exe的静态变量。
5. 即使在dll内部定义了基础数据类型，static变量会存在“全局数据区”，这个地方也是dll内部的PE结构，因此，若是进程A和B加载dll以后，dll的基址都是0x12345678，那么，static char[1000]存放在全局数据区，其地址可能也是一样的，0x12349999，看起来像是两个进程操作着同一个地址，但这个地址位于两个进程的虚拟内存空间中，是隔离的，修改A.exe -- Data.dll -- static char[1000]并不会影响B.exe -- Data.dll里面的数据。
6. 进程的虚拟空间都是隔离的，搞清楚这一点就明白了。

## win静态变量-2
Test.exe  
$~~$|_ A.dll  
$~~~~~~$|_ CC.dll  
$~~~~~~~~~~$|_ static ClassObj  
$~~~~~~~~~~~~~$|_ int m_nData
$~~$|_ B.dll  
$~~~~~~$|_ CC.dll  
$~~~~~~~~~~$|_ static ClassObj  
$~~~~~~~~~~~~~$|_ int m_nData

1. 进程Test.exe先后加载A.dll和B.dll，A和B dll再加载CC.dll，并通过CC dll暴露的接口去读写数据。
2. 在CC dll内部，定义了一个static ClassObj变量，对外提供接口，供A和B dll调用。
3. 由于CC dll在进程空间中只会映射一份内存，因此，A和B dll实际操作的是同一份内存空间。因此，当A dll修改了static ClassObj的m_nData的时候，实际B dll也会受到影响，B dll再去读m_nData，读到的结果已经是A dll刚刚修改过的了。