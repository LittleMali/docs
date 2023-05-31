# deb包检查
Ubuntu系统中有一些工具可以校验deb包格式是否合法。以下是一些常用的工具：

## dpkg-deb
dpkg-deb是一个命令行工具，可以创建、提取和管理deb包。例如，要检查名为“package.deb”的deb包文件的格式是否正确，您可以使用以下命令：  
`dpkg-deb --info package.deb`  
如果deb包格式不正确，dpkg-deb将会显示错误信息。

## lintian
lintian是一个命令行工具，可以检查deb包是否符合Debian软件包政策。您可以使用lintian命令来检查deb包的格式是否正确。例如，要检查名为“package.deb”的deb包文件的格式是否正确，您可以使用以下命令：  
`lintian package.deb`  
如果deb包格式不正确，lintian将会显示错误信息。

## debsums
debsums是一个命令行工具，可以检查deb包中的文件是否被修改或损坏。您可以使用debsums命令来检查deb包的格式是否正确。例如，要检查名为“package.deb”的deb包文件的格式是否正确，您可以使用以下命令：  
`debsums -s package.deb`  
如果deb包格式不正确，debsums将会显示错误信息。
