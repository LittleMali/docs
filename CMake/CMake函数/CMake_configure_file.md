# CMake configure_file

cmake 中的 configure_file 指令通过读取输入文件中的内容，将 CMakeLists.txt 文件中的变量转变为 C/C++ 中可识别的宏定义，然后存入另一个文件中。其语法格式如下。其中，input 为输入的文件，output 为输出的文件。通常，输入文件为 xxx-config.h.in，输出文件为 xxx-config.h。

## 场景
### 判断变量是否定义
`#cmakedefine VAR`  

源码中不关心变量值，只关心变量是否被定义。如果 CMakeLists.txt 文件中，定义了变量 VAR，那么在转化出来的文件中就会存在 #define VAR 的语句。否则，在文件中就会显示 /*undef VAR*/。然后，在源码中使用 #ifdef 语句进行使用——#ifdef VAR。

### 判断选项是否开启
`#cmakedefine01 VAR`  
CMakelists.txt 中的 option(VAR ...) 用于开关操作，并且可以使用 cmake -DVAR=ON/OFF .. 修改其变量值。上述指令根据 CMakeLists.txt 中 VAR 的值为 ON 或 OFF，将其转换为 #define VAR 1 或 #define VAR 0。然后在源码中使用 #if 进行引用——#if VAR。
当然，该语句也可用于使用 set(VAR xxx) 定义的变量，但是用于 option(VAR ...) 更合适。

### 获取变量值
`#cmakedefine VAR @VAR@`  
`#define SELF_DEFINE_MACRO_NAME @VAR@`  
其中的 @VAR@ 可以替换为 ${VAR}。

CMakeLists.txt 文件中，变量 VAR 多用于定义某些信息，比如版本号，作者，项目描述，调试等级等。然后在源码中输出这些值到固定位置，起到提示的作用。

多说一句。如果想要在命令行中修改通过 set() 自定义变量的值，那么需要在定义变量时指定 CACHE 参数。比如：

set(DEBUG_LDEVL 1 CACHE STRING "set debug level")
