
# submodule
git submodule的核心点是父子仓库的关联，关联了一个commit id。
假设父仓库father，里面有子仓库child，父子仓库都有master和dev两条代码线。目录结构如下所示。
--father
&nbsp;&nbsp;--child
## submodule merge
两条分支master和dev，dev如何合入到master中去呢？
* 通常来说master是受保护的，我们没有权限将代码push上去，如果直接将dev-->master，那么一旦有冲突，会很麻烦。所以，通常需要将master先反向同步到dev（master-->dev），这样即使有冲突，冲突也在我们的dev上，可以方便解决。
* 因此，merge其实是两大步，先将master-->dev，再将dev-->master。

假设我们的父子仓库都有两条代码线主线master和分支dev。
一次完整的merge，需要下面N步，好好看！
1. 进入submodule child目录dev分支，将submodule child master-->dev，处理冲突，并将dev push。这样，child dev已经拿到了最新的master代码。
2. 回到father目录，将已修改的child仓库关联id提交并push到远端。这样，父仓库已经关联到了最新的子仓库。
3. 将father master-->dev，并处理冲突。此时的冲突包括两个部分，一个部分是father目录下自身文件的冲突，按需处理即可；另一个部分是父子关联id的冲突，因为我们本地的关联id已经是最新的了，所以，直接选择本地关联id即可。冲突处理完成，push到远端。这样，我们的dev分支就成功merge了master的最新代码。
4. 接下来，要将dev merge回master。正常来说，我们的dev已经是最新的代码，直接merge回master是fast forward，master是肯定没有冲突的。但是，我们可能没有权限将代码push上去，我们需要在git上提交merge申请。那么，这种情况要如何处理呢？
5. 进入submodule child dev，将master代码merge过来，即master-->dev。这一步，我们已经在step1中做过了。
6. 在git平台提交submodule child仓库的merge申请，将dev-->master。
7. 切换到submodule child master，pull拉取刚才的merge请求，此时master已经有了dev的代码。
8. 进入father目录，切换到dev，此时father处于dev分支，child处于master分支，将父子关联id提交并push，这样，father dev分支关联的是child master分支。注意，在这个时刻，父子关联的commit id就是我们所需要的id，这个id对应了**子仓库**最新最全的代码。
9. 在git平台提交father的merge申请，将dev-->master。此时，因为father dev关联的是master最新的commit id，这个id正是我们所需要的id，所以，关联id这里不会有冲突（fast forward）。可能出现冲突的地方是father目录自己的文件，这个步骤，我们之前在step3中处理过了。
