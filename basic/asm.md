ldr: 从存储器中加载

str:将寄存器中内容存入存储器

MRS：将CPSR/SPSR状态[寄存器](https://so.csdn.net/so/search?q=寄存器&spm=1001.2101.3001.7020)读取，保存到R1中

MRS  R1，CPSR



反汇编：

$(CROSS_COMPILE)objdump -S 

bl和b

b跳转后只能用bl跳转回来，长跳转。

https://www.jianshu.com/p/d5a35a659d7f

ARM精简指令集

（注意操作系统位数）

LDR 用于将内存或缓冲区数据传入寄存器

STR   用于将寄存器中传入内存或者缓冲区

MRS 将CPSR/SPSR（程序状态寄存器/程序状态保护寄存器）

的内容读取到通用寄存器中

MSR 将通用寄存器的值写到CPSR/SPSR的特定位域（f，c）中

BL 跳转指令

![img](../typora-user-images/clip_image002.jpg)

TEXT segment dword public use32 'CODE'

_TEXT ends

_DATA segment dword public use32 'DATA'

_DATA ends

_BSS segment dword public use32 'BSS'

_BSS ends

Segment和ends围起来的称为段定义，段定义是一段连续的内存空间。

_TEXT是被指定（代码段）的段定义，_DATA是有初始化的段定义，_BSS是未初始化的段定义。

![img](../typora-user-images/clip_image004.jpg)
