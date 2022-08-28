ldr: 从存储器中加载

str:将寄存器中内容存入存储器

MRS：将CPSR/SPSR状态[寄存器](https://so.csdn.net/so/search?q=寄存器&spm=1001.2101.3001.7020)读取，保存到R1中

MRS  R1，CPSR



反汇编：

$(CROSS_COMPILE)objdump -S 

bl和b

b跳转后只能用bl跳转回来，长跳转。

