# Linux文件监控

Linux内核提供几个监控机制有inotify，fanotify，audit。其中inotify和fanotify主要是文件系统的监控，也就是监控文件访问，audit是系统调用的审核服务，也支持文件监控。

## inotify
Linux内核自2.6.13版本开始引入，主要是监控文件操作。
* 访问（IN_ACCESS）：文件被访问（读取）。
* 修改（IN_MODIFY）：文件被修改。
* 属性改变（IN_ATTRIB）：文件的元数据（例如权限、时间戳、链接数等）被修改。
* 关闭写入（IN_CLOSE_WRITE）：可写文件被关闭。
* 关闭非写入（IN_CLOSE_NOWRITE）：不可写文件被关闭。
* 打开（IN_OPEN）：文件被打开。
* 移动（IN_MOVED_FROM, IN_MOVED_TO）：文件被移动或重命名。
* 创建（IN_CREATE）：在监控的目录中创建了新文件或新目录。
* 删除（IN_DELETE）：在监控的目录中删除了文件或目录。
* 自删除（IN_DELETE_SELF）：被监控的文件或目录被删除。
* 自移动（IN_MOVE_SELF）：被监控的文件或目录被移动。

注意：inotify最大的问题是，拿不到进程pid，也就是不知道是谁操作了文件。同时，inotify只能监控指定目录及其下一级子目录，无法再覆盖下下级子目录。

## fanotify
fanotify是inotify的替代版，从内存2.6.36版本引入。相对inotify而言，fanotify的优势有：
* inotify是旁路监听，fanotify可以对文件操作进行阻断。
* inotify只能监控指定目录及其下一级子目录的文件变化，再往下的子目录就不行了，fanotify可以监控整个指定目录，包括其下所有子目录的变化。