# git tag

* 查看tag  
`git tag`  
`git tag -l`  
`git tag -l 'v5.12.*'`

* 查看tag详情  
`git show v5.12.8`

* 切换到某个tag  
`git checkout -b myBranchName v.12.8`  
直接切换到 tag v5.12.8，可以查看代码，但是不能修改&&提交，因为checkout以后等价于切换到某一次提交记录，类似于看快照。  
`git checkout -b myBranchName v.12.8`  
切换到某个tag，同时创建一个分支，在此分支上修改代码。

