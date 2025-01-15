# log相关操作

## 修改已提交的log
小乌龟git只能修改最近一次提交log，如果再翻之前的记录，不知道在哪里改。下面统一使用git bash好了。
1. 最后一次提交且未push
```
  git commit --amend
```
修改log，保存并退出，即可完成此次修改。

2. 最后一次提交且已经push到服务器
```
  git commit --amend
  git push --force
```
修改log的做法与情况1相同。但是由于log已经被到远端了，所以，这个时候必须--force强推上去。
这种做法有风险，可能影响其他人的提交，在push之前要跟他们确认一下。

3. 旧的提交且未push
```
// 确认一下要修改哪一次的提交
git log 

// 把最近3次的提交显示出来
git rebase -i HEAD~3 
// 会显示如下内容，这里从上到下按照时间顺序，最早提交的排在最前面。
// 第一列表示命令pick，第二列表示commit id，第三列是我们的log内容。
pick 94fc8fe 添加工程文件10月1号
pick 04f0d18 添加代码10月2号
pick b1b451d 添加图片10月3号

// 假设我们要修改多行行log，将光标定位到第n行，并将pick修改为edit。
// 然后保存并退出。
pick 94fc8fe 添加工程文件10月1号
die 04f0d18 添加代码10月2号 // edit!!!
edit b1b451d 添加图片10月3号 // edit!!!

// 接下来就是修改log内容，跟情况1类似。
git commit --amend

// 修改完log之后，继续修改下一个
git rebase --continue

// 几个log都修改完了，会显示下面的内容
> Successfully rebased and updated refs/heads/xxx.
```

4. 旧的提交且已push
场景4跟场景3是类似的，修改log记录，然后强推到远端。
```
git log
git rebase -i HEAD~X
// pick --> edit
git commit --amend
git rebase --continue

// 最后推送到远端服务器
git push --force
```
