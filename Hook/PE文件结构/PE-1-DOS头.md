# PE DOS头

## PE指纹
判断一个文件是否是PE文件的依据，首先是根据文件的前两个字节是否为4D 5A，也就是’MZ’，然后看第四排四个字节指向的地址00 00 00 f8是否为50 45，也就是’PE’，满足这两个条件也就满足了PE文件的格式，简称PE指纹。

## DOS头
DOS部分主要是为了兼容以前的DOS系统，DOS部分可以分为DOS MZ文件头(IMAGE_DOS_HEADER)和DOS块(DOS Stub)组成，PE文件的第一个字节位于一个传统的MS-DOS头部，称作IMAGE_DOS_HEADER，其结构如下：
```
typedef struct _IMAGE_DOS_HEADER {      // DOS .EXE header
    WORD   e_magic;                     // Magic number
    WORD   e_cblp;                      // Bytes on last page of file
    WORD   e_cp;                        // Pages in file
    WORD   e_crlc;                      // Relocations
    WORD   e_cparhdr;                   // Size of header in paragraphs
    WORD   e_minalloc;                  // Minimum extra paragraphs needed
    WORD   e_maxalloc;                  // Maximum extra paragraphs needed
    WORD   e_ss;                        // Initial (relative) SS value
    WORD   e_sp;                        // Initial SP value
    WORD   e_csum;                      // Checksum
    WORD   e_ip;                        // Initial IP value
    WORD   e_cs;                        // Initial (relative) CS value
    WORD   e_lfarlc;                    // File address of relocation table
    WORD   e_ovno;                      // Overlay number
    WORD   e_res[4];                    // Reserved words
    WORD   e_oemid;                     // OEM identifier (for e_oeminfo)
    WORD   e_oeminfo;                   // OEM information; e_oemid specific
    WORD   e_res2[10];                  // Reserved words
    LONG   e_lfanew;                    // File address of new exe header
  } IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;
```

DOS部分我们需要熟悉的是e_magic成员和e_lfanew成员，前者是标识PE指纹的一部分，后者则是寻找PE文件头的部分，除了这两个成员，其他成员全部用0填充都不会影响程序正常运行，所以我们不需要过多的对其他部分深究。
* e_magic：一个 WORD 类型，值是一个常数 0x4D5A，用文本编辑器查看该值位‘MZ’，可执行文件必须都是'MZ'开头。
* e_lfanew：为 32 位可执行文件扩展的域，用来表示 DOS头之后的 NT头相对文件起始地址的偏移。