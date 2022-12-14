```
https://juejin.cn/post/7139543023557279780
```

make可以找出修改过的文件，根据依赖关系，找出受影响的文件，最后根据规则单独编译这些文件。

makefile\Makefile:记录依赖关系和编译规则

makefile本质：面向依赖

三要素：目标、依赖、命令

目标：依赖的文件或者其他目标

<tab>命令1

<tab>命令2

<tab>命令3

<tab>……

.PHONY:targetb（指定伪目标）表示不需要targetb这一目标文件，保证每次执行就算改文件不修改也能执行，要执行创建的伪目标需要make target

make or make -f file

![image-20220827191230187](../typora-user-images/image-20220827191230187.png)

ifeq (a,b) or ifneq

else ifeq(a,b)

else

endif

函数：

$(filter pattern, text)  //在text中取出符合pattern格式的值

$(filter-out pattern, $(C))   //在text中取出不 符合pattern格式的值

$(patsubst %.c,%.o,x.c.c bar.c)

把x.c.c、bar.c改成x.c.o bar.o ，用来生成目标文件和依赖文件

 

$(notdir src/foo.c hacks)返回为  foo.c hacks 意为除去目录，经常用来生成目标文件

dirs := a b c d

files := $(foreach dir,$(dirs),$(wildcard $(dir)/*))

输出：a,b,c,d目录下的所有文件

foreach 用于多个目录的寻访  

wildcard用于找目录下的特定文件  wildcard dir/*.c or /*.h

链接问题   gcc -c *.c -o *.o -I include_dir(这里是目录不是文件)

D:\a_youwant\LINUX\100ask\01_all_series_quickstart\04_嵌入式Linux应用开发基础知识\source\05_general_Makefile\Makefile_and_readme

D:\a_youwant\LINUX\100ask\01_all_series_quickstart\04_嵌入式Linux应用开发基础知识\doc_pic\04.2018_Makefile



```
# 1. 使用不同的开发板内核时, 一定要修改KERN_DIR
# 2. KERN_DIR中的内核要事先配置、编译, 为了能编译内核, 要先设置下列环境变量:
# 2.1 ARCH,          比如: export ARCH=arm64
# 2.2 CROSS_COMPILE, 比如: export CROSS_COMPILE=aarch64-linux-gnu-
# 2.3 PATH,          比如: export PATH=$PATH:/home/book/100ask_roc-rk3399-pc/ToolChain-6.3.1/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin 
# 注意: 不同的开发板不同的编译器上述3个环境变量不一定相同,
#       请参考各开发板的高级用户使用手册
KERN_DIR = /home/liujun/ftp_dir/SDK/lbd3399-lubuntu-sdk-v1/kernel

all:
	make -C $(KERN_DIR) M=`pwd` modules

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order
#CFLAGS_st7735s.o += -DCONFIG_FB_DEFERRED_IO
EXTRA_CFLAGS += -DCONFIG_FB_DEFERRED_IO
obj-m	+= st7735s.o
```

`make -C $(KERN_DIR) M=pwd modules`

`-C`指定执行make所在目录

`M=pwd`在当前文件夹寻找源代码并且生成模块



`obj-m`: 编译成模块

`obj-y`: 编译进内核



在Makefile中往linux内核添加宏定义

- `CFLAGS_obj.o += -DCONFIG_FB_DEFERRED_IO`往自己的中添加宏定义
- `EXTRA_CFLAGS += -DCONFIG_FB_DEFERRED_IO`全局宏定义



默认变量

默认变量是Makefile的约定，比如：

```bash
test:   $(CC) -o test test.c
复制代码
```

其中CC就是一个默认变量，在linux下就是编译器cc。其它比较常用的默认变量如下：

关于命令相关的变量

- `AR` : 函数库打包程序。默认命令是 `ar`
- `AS` : 汇编语言编译程序。默认命令是 `as`
- `CC` : C语言编译程序。默认命令是 `cc`
- `CXX` : C++语言编译程序。默认命令是 `g++`
- `CO` : 从 RCS文件中扩展文件程序。默认命令是 `co`
- `CPP` : C程序的预处理器（输出是标准输出设备）。默认命令是 `$(CC) –E`
- `FC` : Fortran 和 Ratfor 的编译器和预处理程序。默认命令是 `f77`
- `GET` : 从SCCS文件中扩展文件的程序。默认命令是 `get`
- `LEX` : Lex方法分析器程序（针对于C或Ratfor）。默认命令是 `lex`
- `PC` : Pascal语言编译程序。默认命令是 `pc`
- `YACC` : Yacc文法分析器（针对于C程序）。默认命令是 `yacc`
- `YACCR` : Yacc文法分析器（针对于Ratfor程序）。默认命令是 `yacc –r`
- `MAKEINFO` : 转换Texinfo源文件（.texi）到Info文件程序。默认命令是 `makeinfo`
- `TEX` : 从TeX源文件创建TeX DVI文件的程序。默认命令是 `tex`
- `TEXI2DVI` : 从Texinfo源文件创建TeX DVI 文件的程序。默认命令是 `texi2dvi`
- `WEAVE` : 转换Web到TeX的程序。默认命令是 `weave`
- `CWEAVE` : 转换C Web 到 TeX的程序。默认命令是 `cweave`
- `TANGLE` : 转换Web到Pascal语言的程序。默认命令是 `tangle`
- `CTANGLE` : 转换C Web 到 C。默认命令是 `ctangle`
- `RM` : 删除文件命令。默认命令是 `rm –f`

关于命令参数的变量

- `ARFLAGS` : 函数库打包程序AR命令的参数。默认值是 `rv`
- `ASFLAGS` : 汇编语言编译器参数。（当明显地调用 `.s` 或 `.S` 文件时）
- `CFLAGS` : C语言编译器参数。
- `CXXFLAGS` : C++语言编译器参数。
- `COFLAGS` : RCS命令参数。
- `CPPFLAGS` : C预处理器参数。（ C 和 Fortran 编译器也会用到）。
- `FFLAGS` : Fortran语言编译器参数。
- `GFLAGS` : SCCS “get”程序参数。
- `LDFLAGS` : 链接器参数。（如： `ld` ）
- `LFLAGS` : Lex文法分析器参数。
- `PFLAGS` : Pascal语言编译器参数。
- `RFLAGS` : Ratfor 程序的Fortran 编译器参数。
- `YFLAGS` : Yacc文法分析器参数

### 通过 -B 选项让所有目标总是重新建立

到目前为止，你可能注意到 make 命令不会编译那些自从上次编译之后就没有更改的文件，但是，如果你想覆盖 make 这种默认的行为，你可以使用 -B 选项。

###  使用 -d 选项打印调试信息

```
$ make -d | more
GNU Make 3.81
Copyright (C) 2006 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.
There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.

This program built for x86_64-pc-linux-gnu
Reading makefiles…
Reading makefile `Makefile’…
Updating makefiles….
Considering target file `Makefile’.
Looking for an implicit rule for `Makefile’.
Trying pattern rule with stem `Makefile’.
Trying implicit prerequisite `Makefile.o’.
Trying pattern rule with stem `Makefile’.
Trying implicit prerequisite `Makefile.c’.
Trying pattern rule with stem `Makefile’.
Trying implicit prerequisite `Makefile.cc’.
Trying pattern rule with stem `Makefile’.
Trying implicit prerequisite `Makefile.C’.
Trying pattern rule with stem `Makefile’.
Trying implicit prerequisite `Makefile.cpp’.
Trying pattern rule with stem `Makefile’.
--More--
```

### 使用 -C 选项改变目录

### 通过 -f 选项将其它文件看作 Makefile

如果你想将重命名 Makefile 文件，比如取名为 my_makefile 或者其它的名字，我们想让 make 将它也当成 Makefile，可以使用 -f 选项。

```
make -f my_makefile
```

通过这种方法，make 命令会选择扫描 my_makefile 来代替 Makefile。
