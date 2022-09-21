## buildroot构建根文件系统

```
https://buildroot.org/
```

图形化界面配置 make menuconfig

基于busybox制作

### 1、配置 Target options

配置框架（ARM/ARM64 little/big endian）

二进制文件形式

内核（cortex-a7/a72....）

### 2、配置 Toolchain

配置交叉编译工具链

工具链名字，使用的工具链类型、版本、C++support\MMU support and so on

### 3、配置 System configuration

开发板名字、欢迎语、用户名和密码等

### 4、配置 Filesystem images

根文件系统文件格式

### 5、禁止编译 Linux 内核和 uboot

虽然buildroot里面可以编译linux内核和uboot，但是下载的内核和uboot会少很多文件，并且最新的内核和uboot对编译器版本有要求，可能会导致编译失败。

### 6、配置 Target packages

用于配置ffmpeg\bzip\QT等

### buildroot 下的 busybox 配置

buildroot/dl/busybox中找到busybox的压缩包

/output/build/busybox解压缩后的busybox存放目录

```

sudo make busybox-menuconfig
    make busybox	//单独编译busybox
```

