
# submodule
git submodule的核心点是父子仓库的关联，关联了一个commit id。  
假设父仓库father，里面有子仓库child，父子仓库都有master和dev两条代码线。目录结构如下所示。  
--father  
&nbsp;&nbsp;--child

## submodule使用
* 添加submodule  
  在父仓库中使用 `git submodule add Git_URL child_dir`
* 父仓库的时候同步拉取子仓库  
  `git clone Git_URL --recurse-submodules`
* 单独拉取子仓库  
  `git submodule update --init --recursive`
* 查看子仓库的状态  
  在父仓库中使用 `git submodule status --recursive`
* 限定子仓库版本
  1. 父仓库通常是关联到子仓库的某个tag而不是branch，因为tag一般表示某个稳定版本，比branch更合适。所以，submodule在设计的时候，父子仓库之间通过commit id来关联，也就是说当我们把子仓库拉下来以后，子仓库是处于“no branch”状态的。即使我们在子仓库中切换了分支git checkout master这样的命令，父仓库也不会记录分支的关联性，父仓库只会记录当前master的commit id。
  2. 由于父子之间关联的是commit id，那么，当我们要更新submodule时，比如官方更新了一个新版本，那么，我们可以这么做。
  ```
  cd child
  git pull # 这一步能看到子仓库的变动，被拉到了本地
  git checkout curl-7_83_1 # 切换到curl的7.83.1版本

  cd ../
  git submodule status
    +462196e6b4a47f924293a0e26b8e9c23d37ac26f child (curl-7_83_1)

  # 子仓库已经发生了变动，注意前面的+号。

  #提交修改
  git commit -m "update curl"
  git push
  ```
  3. 当同事更新了submodule，我要怎么获取到同事的修改呢？  
  父仓库 `git pull`  
  父仓库 `git submodule update child`，这里是显示的限定了只更新child子仓库。如果父仓库关联了多个子仓库，想一次性更新全部的子仓库，直接用`git submodule update`。


## submodule merge
两条分支master和dev，dev如何合入到master中去呢？
* 通常来说master是受保护的，我们没有权限将代码push上去.
* 如果在git上提merge单，直接将dev-->master，那么一旦有冲突，冲突会体现在master上，而我们又没有权限修改master代码，这就很麻烦了。所以，通常需要将master先反向同步到dev（master-->dev），这样即使有冲突，冲突也在我们的dev上，可以方便解决。
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


1. 本地进入submodule child目录，切换到dev分支，将submodule child master-->dev，处理冲突，并将child dev push。这样，child dev已经是最新的代码，并且没有冲突了。
2. 在git平台上，进入submodule child仓库，提交merge单，将child dev-->master，因为我们之前在本地child dev反向同步过child master代码，所以，直接提merge单不会有冲突发生，不用担心child master会有问题。
3. 至此，子仓库处理完了，我们再来处理父仓库。
4. 本地进入submodule child目录，切换到master分支，并pull拉取刚刚提交的merge内容。
5. 本地进入father目录，切换到dev分支，此时，我们先想一想关联id。father目录是dev分支，submodule child目录是master分支，那么，我们的father dev实际关联了child master，这个关联关系就是我们最后所期待的。如果整个merge完成，我们的父仓库就应该关联到子仓库的master提交id。注意，这个时候，我们还没有commit&&push这个最终正确的关联id。
6. 将father master-->dev，处理father目录下的冲突。注意，冲突只有father自身代码的冲突。
7. 父仓库push，push的同时一并推送关联id。
8. 在git平台上，进入father仓库，提交merge单，将father dev-->master，因为本地反向同步过了，所以，merge以后master分支不会有冲突。并且，merge以后，master分支将会拿到father dev push上来的关联id，这个id恰恰是child master的commit id。