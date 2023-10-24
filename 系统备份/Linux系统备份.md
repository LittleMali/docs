# Linux系统备份
rsync是一个在Unix-like系统（包括Linux，如Ubuntu）中常用的数据复制和同步工具。它可以用来备份文件，同步文件，以及镜像整个文件系统。rsync是开源的，并且通常会预装在许多Linux发行版中，包括Ubuntu。

rsync的主要优点是它只传输文件的变化部分，而不是整个文件。这使得rsync在处理大文件或者大量文件时非常高效，特别是当这些文件自上次复制以来只有少量改动的情况下。

虽然rsync可以用来备份文件，但是Ubuntu系统本身并没有默认使用rsync来备份文件。Ubuntu有自己的备份工具叫做Déjà Dup，它提供了图形用户界面，可以用来备份文件和文件夹，以及恢复备份。Déjà Dup在后台使用了另一个命令行工具叫做duplicity，而不是rsync。

然而，许多Ubuntu用户和系统管理员会选择使用rsync来备份文件，因为rsync提供了更多的灵活性和控制，尤其是在命令行环境中。

## rsync工具
* git仓库： https://github.com/WayneD/rsync
* 官网： https://rsync.samba.org/
* rsync shell封装：https://github.com/laurent22/rsync-time-backup

