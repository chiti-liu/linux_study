# git note
my linux study note, including kernel/u-boot/qt and so on

### git command

#### 创建账户

注：如果是**新登录电脑**请注意密钥和权限问题

```
git config --global user.name "username"	//绑定用户名
git config --global user.email "email"		//绑定密码

git config user.name   // 获取当前登录的用户
git config user.email  // 获取当前登录用户的邮箱

git config --global user.name 'userName'    // 修改登陆账号
git config --global user.email 'email'      // 修改登陆邮箱
git config --global user.password 'password'  // 修改登陆密码

```

#### 新建reponsitory

github首先创建reponsitory

个人终端

```
git init //把这个目录变成Git可以管理的仓库
git add . //不但可以跟单一文件，还可以跟通配符，更可以跟目录。一个点就把当前目录下所有未追踪的文件全部add了，添加到暂存区
git commit -m "first commit" // git add 命令将内容写入暂存区。git commit 命令将暂存区内容添加到本地仓库中，-m标注
git remote add origin *** //关联远程仓库
git push -u origin master //把本地库的所有内容推送到远程库上，-u指定默认主机，以后就可以不用加-u默认上传master
```

#### 创建新分支

```
git status	//查看当前所在分支
git branch -a	//查看分支
git branch test	//创建一个分支test
git checkout test	//master分支是仓库默认的主分支，把工作从master分支下切换到test分支下

//git checkout -b test	//创建并切换test工作分支

//内容修改完成后，通过下面命令把内容提交给test分支下
git add .
git commit -m "test commit" //git add 命令将内容写入暂存区。git commit 命令将暂存区内容添加到本地仓库中，-m标注
git push -u origin test
```

#### 单独上传更新文件或分支

```
git diff  //比较文件在暂存区和工作区的差异,工作区即仓库
git add . //不但可以跟单一文件，还可以跟通配符，更可以跟目录。一个点就把当前目录下所有未追踪的文件全部add了 
git commit -m "fix" //把文件提交到仓库并标注
git remote add origin *** //关联远程仓库
git push -u origin master //把本地库的所有内容推送到远程库上
```

#### 拉取更新

##### 在此之前需要搞清楚git pull 和 git fetch区别

在我理解：如果是**个人开发**，完全可以使用git pull

如果是**多人开发**，那么git fetch+git merge就非常有必要，可以在git fetch 更新本地仓库时不更新本地目录，并且查看距离上次更新产生了哪些变化。

```

git pull origin master	//暴力更新，不会有中间步骤给你核实吗，只适合个人开发
git fetch	//拉取remote仓库到本地仓库，但不更新目录
git merge	//提交并合并remote仓库
```

#### 合并分支

```
git status	//查看当前所在分支
git branch -a	//可以查看当前仓库和remote的分支信息
git checkout master	//工作从其它分支下切换到master下
git fetch	//因为是合作开发项目，这时远程仓库中的内容有可能已经发生了变化，所以我们需要先合并仓库，并查看有哪些改变了，方便我们排除错误和后悔
git merge	//合并更新本地仓库和remote仓库
git merge test	//将test分支合并到主分支中S
git status	//查看分支中内容提交的状态
git push origin master

git reset --hard HEAD	//如果感觉合并后的内容有问题，可以通过撤销合并恢复到以前状态。
git reset --hard ORIG_HEAD	//代码已经提交，撤销的方法是
git branch -d 	//删除节点
```

##### 疑问：如果当前master分支还未更新，直接合并其它分支到主分支会发生什么？

如果当前修改的文件和remote仓库中文件修改到了同一行（本地还未更新，其他人提交了更新），那么将会造成冲突生成节点3，出错了会很难找问题。

### git trouble

#### 密钥和权限问题

- 如何解决 git@github.com permission denied (publickey). fatal could not read from remote repository

  ```
  https://zhuanlan.zhihu.com/p/454666519
  ```

  Github 拒绝连接，其原因有两个

  1. 这是你的私人仓库(repo)
  2. Github 不信任你的计算机，因为它没有你计算机的公钥(public key)

  - 第一步：创建SSH Key pair

    打开git bash，输入

    ```
    ssh-keygen
    ```

    提示输入秘钥名字和密码，为了方便起见，我这里选择默认地址。

    到文件地址C:\Users\liujun\.ssh中 id_rsa.pub复制密钥。

  - 第二步：将SSH key添加到github账户

    github >> setting >>SSH and GPG keys >> new SSH key >>Add SSH key


#### git加速问题

```
https://qu.js.cn/how-to-speed-github/
```

windows系统中修改C:\Windows\System32\drivers\etc\hosts文件的权限，指定可写入；

用编辑器打开hosts文件，在末尾处添加以下内容：

```
199.232.69.194 github.global.ssl.fastly.net
140.82.112.4 github.com
```

#### git bash here not found

```
https://www.cnblogs.com/xinxin1994/p/10422720.html
```

1、Win+R 打开运行输入regedit 回车打开注册表

2、找到[HKEY_CLASSES_ROOT\Directory\Background]。

3、在[Background]下如果没有[shell],则右键-新建项[shell]。

4、在[shell]下右键-新建项[Git Bash Here],其值为“Git Bash Here",此为右键菜单显示名称。

   此时在任意位置鼠标右击就能看到Git Bash Here但是没有关联程序，现在还没有实际作用

5、 在[Git Bash Here]下右键-新建-项[command],其默认值为 "git安装根目录\bin\bash.exe"

6、再完善一下添加一个Git的小图标：选中Git Bash Here右击-新建-字符串值，名称为Icon,双击编辑，

   其值为“git安装根目录\\mingw64\share\git\git-for-windows.ico”。

#### unable to access

```
https://blog.csdn.net/good_good_xiu/article/details/118567249
```

命令行输入

```
//取消http代理
git config --global --unset http.proxy
//取消https代理 
git config --global --unset https.proxy
```

