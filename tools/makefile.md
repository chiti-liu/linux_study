

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
