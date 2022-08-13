# CMake命令

## win下使用cmake
1. 下载并安装：https://cmake.org/download/
2. 进入工程目录，创建build目录。
3. cd build
4. cmake ..
5. cmake --build .

网上的例子：
`cmake -S E:\CmakeTest -B E:\CmakeTest\build -G "Visual Studio 16 2019" -A x64`
`cmake --build  E:\\CmakeTest\build`
* -S：顶级CMakeLists.txt（包含project声明）所在路径。
* -B：存放临时编译的二进制文件（.obj、.ilk等）和编译器对应的配置文件路径。
* -G：编译器名称。`cmake -G`可以查看支持哪些编译器。
* -A：架构名称。
* -D：使用该变量以向cmake传入各种参数。包括选项及覆盖cmake提供的各种默认变量值。
* --build：使用cmake直接调用编译器编译项目。