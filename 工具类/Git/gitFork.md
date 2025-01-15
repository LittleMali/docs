# git fork

git fork是从别人的仓库拉一个镜像到自己的git仓库，自己在自己的git仓库修改代码。  
git fork一般是用在自己没有某个仓库的开发者权限，但是又希望修改源码，并且能够跟源仓库保持代码同步。  
git fork的示意图如下。
![20220424200124](https://raw.githubusercontent.com/LittleMali/docs/master/mdPics/20220424200124.png)

## 用法
1. fork源仓库。
  例如，在开源openssl的git仓库，fork到自己的git -- 命名为openssl-test。
![20220424200702](https://raw.githubusercontent.com/LittleMali/docs/master/mdPics/20220424200702.png)

2. git clone自己的仓库。
  `git clone https://github.com/LittleMali/openssl-test.git`

3. 基于开源代码继续开发。  
  修改代码。  
  `git add`。  
  `git commit`。  
  `git push`。   
  我们这些修改，都是在自己的git仓库 -- openssl-test。  
  即使是多人协作开发，多人commit && push，都是操作的镜像仓库 -- openssl-test。 

4. 经过一段时间以后，开源openssl出了新版本，我们希望同步openssl官方的代码。  
  核心操作是 拉取官方仓库并merge到自己的分支。  
  说明：当我们git clone openssl-test到本地时，我们的git仓库默认关联的远端分支是自己的origin openssl-test。
```bash
# 查看当前代码关联的远端分支。
$ git remote -v
origin  https://github.com/LittleMali/openssl-test.git (fetch)
origin  https://github.com/LittleMali/openssl-test.git (push)

# 添加官方git仓库的远端关联。
# myupstream是我们自己起的一个名字，可以随便叫。
git remote add myupstream https://github.com/LittleMali/openssl-test.git

# 再次查看关联
$ git remote -v
myupstream      https://github.com/LittleMali/openssl-test.git (fetch)
myupstream      https://github.com/LittleMali/openssl-test.git (push)
origin  https://github.com/LittleMali/openssl-test.git (fetch)
origin  https://github.com/LittleMali/openssl-test.git (push)

# pull官方openssl的修改
# 这里要指明拉取的远端分支名称。
# 一般我们简写git pull的时候，是默认git pull origin <cur-branch-name>。
$ git pull myupstream master
From https://github.com/LittleMali/openssl-test
 * branch                  master     -> FETCH_HEAD
Already up to date.

# 这个时候，如果有冲突，则需要我们接冲突。

# 冲突处理完成，推送代码到我们自己的仓库 -- openssl-test。
git push
```
  说明：在上面的bash操作中，我们的openssl-test处于master分支，官方openssl仓库也是master分支，我们直接 `git pull myupstream master`的时候，git会提示冲突。  
  当然，我们上面是一个很简单的例子，实际开发的时候，会有feature和release等分支，其本质就是pull myupstream的代码，然后merge。

5. 假设我们代码很好，想提交到官方openssl仓库，使用pull request。  
  没有查到git pull request的命令，pull request是在web页面操作的。  
  从经验上看，我们是把自己的修改提交到官方仓库，应该叫push更合适，但是，git却称之为pull request，网上有一个说法，大概的意思是，pull request是从官方openssl的角度来看的，官方仓库有权决定是否拉取我们的修改，所以，是pull request。

![20220424204223](https://raw.githubusercontent.com/LittleMali/docs/master/mdPics/20220424204223.png)
