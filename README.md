# linux_study
my linux study note, including kernel/u-boot/qt and so on

### git command





### git trouble

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

    到文件地址 /c/Users/liujun/.ssh/id_rsa.pub中复制密钥。

  - 第二步：将SSH key添加到github账户

    github >> setting >>SSH and GPG keys >> new SSH key >>Add SSH key

- git access

  ```
  https://qu.js.cn/how-to-speed-github/
  ```

  windows系统中修改C:\Windows\System32\drivers\etc\hosts文件的权限，指定可写入；

  用编辑器打开hosts文件，在末尾处添加以下内容：

  ```
  199.232.69.194 github.global.ssl.fastly.net
  140.82.112.4 github.com
  ```

  

