根据原理图指定方案，确定冲突引脚并且**失能**冲突功能，开启时钟。

### Framebuffer

#### deferred io机制

要使能`deferred io机制`， 要打开`CONFIG_FB_DEFERRED_IO`配置

```
https://www.cnblogs.com/vedic/p/10722514.html
```

`deferred io`机制主要用于驱动没有实现自刷新（可以`dma`&定时器配合开新线程实现）同时应用层又不想调用`FBIOPAN_DISPLAY`的一个折中方案。

`ioctrl FBIOPAN_DISPLAY`好处是节能， 驱动不用盲目的刷数据（尤其是一静态帧数据）， 数据的更新是由应用程序操作的，所以应用程序当然知道何时刷数据， 最理想的情况是应用程序一更新数据立马调用`FBIOPAN_DISPLAY`， 但也有缺点， 一是要应用层显示调用`FBIOPAN_DISPLAY`，二是画面更新频率高的话， `FBIOPAN_DISPLAY`带来的系统调用开支也不小；

使用驱动自刷新当然解放应用， 应用不用关心数据显示问题， 直接操作显存， 所写即所见。

**（不全）**

#### fb_info

```
https://blog.csdn.net/weixin_42381087/article/details/116578292
https://blog.csdn.net/zhaohc_nj/article/details/50095995
```

```
fb_info = framebuffer_alloc(SIZE, dev)	
	muxtex_init();
	fb_deferred_io = devm_kzalloc	//可选，参照deferred io机制章节
	info填充
		fb_deferred_io		//可选，参照deferred io机制章节
		fb_var_screeninfo  	//可变参数,分辨率，RGB像素、透明度等
		fb_fix_screeninfo	//固定参数
		fb_ops				//画板操作
		cmap				//调色板信息
		screen_base			//u8 __force __iomem *类型，可以直接dma不用强制转换
		//info->screen_base = dma_alloc_writecombine(NULL, info->fix.smem_len, &fix.smem_start,GFP_KERNEL);
		fix.smem_start		//需要映射到物理地址
		fix.smem_len
		screen_size
		fix.line_length		//注意参数的正确，需要保证不会出现segment fault的错误
		****
	fb_deferred_io_init
	
	/* 分配内核刷新页面空间 */
	
register_framebuffer
```

```
register_framebuffer
	do_register_framebuffer
		device_create(fb%d)
		fb_init_device
			dev_set_drvdata(fb_info->dev, fb_info);
			device_create_file
		fb_var_to_videomode
```

#### dma_alloc_writecombine & dma_alloc_coherent

```
https://blog.csdn.net/manshq163com/article/details/18451135
```

```
 /*
  * Allocate DMA-coherent memory space and return both the kernel remapped
  * virtual and bus address for that space.
  */  
    void *  dma_alloc_coherent(struct device *dev, size_t size, dma_addr_t *handle, gfp_t gfp)  
{  
    void *memory;  
      
    if (dma_alloc_from_coherent(dev, size, handle, &memory))  
    return memory;  
      
    if (arch_is_coherent()) {  
        void *virt;  

        virt = kmalloc(size, gfp);  
        if (!virt)  
        return NULL;  
        *handle =  virt_to_dma(dev, virt);  

        return virt;  
        }  
      
    return __dma_alloc(dev, size, handle, gfp,  
    pgprot_noncached(pgprot_kernel));  
}  

```

```
 /*
    * Allocate a writecombining region, in much the same way as
    * dma_alloc_coherent above.
    */  
    void *  
    dma_alloc_writecombine(struct device *dev, size_t size, dma_addr_t *handle, gfp_t gfp)  
    {  
        return __dma_alloc(dev, size, handle, gfp,  
        pgprot_writecombine(pgprot_kernel));  
    }  
      
      
    #define pgprot_noncached(prot)  __pgprot(pgprot_val(prot) &~(L_PTE_CACHEABLE | L_PTE_BUFFERABLE))  
    #define pgprot_writecombine(prot) __pgprot(pgprot_val(prot) &~L_PTE_CACHEABLE)  

```

`dma_alloc_coherent` 在 arm 平台上会禁止页表项中的 `C （Cacheable）` 域以及 `B (Bufferable)`域。而 `dma_alloc_writecombine` 只禁止 `C （Cacheable）` 域.

 **C 代表是否使用高速缓冲存储器， 而 B 代表是否使用写缓冲区。**

这样，`dma_alloc_writecombine` 分配出来的内存不使用缓存，但是会使用写缓冲区。而 `dma_alloc_coherent`  则二者都不使用。
C B 位的具体含义：

- `0 0` 无cache，无写缓冲；

  任何对memory的读写都反映到总线上。

  对 memory 的操作过程中CPU需要等待。

- `0 1` 无cache，有写缓冲；

  读操作直接反映到总线上；

  写操作，CPU将数据写入到写缓冲后继续运行，由写缓冲进行写回操作。

- `1 0` 有cache，写通模式；

  读操作首先考虑cache hit；

  写操作时直接将数据写入写缓冲，如果同时出现cache hit，那么也更新cache。

- `1 1` 有cache，写回模式；

  读操作首先考虑cache hit；

  写操作也首先考虑cache hit。

#### mmap

`\#include <sys/mman.h>`

```
void *mmap(void *addr, size_t length, int prot, int flags,
        int fd, off_t offset);
int munmap(void *addr, size_t length);
```

mmap实现了将设备驱动在内核空间的部分地址**直接映射**到用户空间，使得用户程序可以**直接访问和操作**相应的内容。**减少了额外的拷贝**，而一般的read，write函数虽然表面上直接向设备写入，其实**还需要进行一次拷贝**。

```
https://blog.csdn.net/manshq163com/article/details/18451135
```

```
mmap
void *mmap(void *addr, size_t length, int prot, int flags,
        int fd, off_t offset);
```

把文件或者设备映射到内存。

这个函数在调用进程的虚拟地址空间中创建一块映射区域。

**这个函数返回新创建的页面的地址。**

映射区域的首地址在addr中指定，length指定映射区域的长度。如果addr是NULL，那么**由内核来选择一个地址**来创建映射的区域,否则创建的时候会尽可能地使用addr的地址。在linux系统中，创建映射的时候应该是在下一个页面的边界创建，**addr是NULL的时候，程序的可移植性最好**。
length指定文件被映射的长度。

offset指定从文件的哪个偏移位置开始映射**，offset必须是页面大小的整数倍**，页面的大小可以由`sysconf(_SC_PAGE_SIZE)`来返回。
prot指定内存的保护模式（具体参见man）。
flags指定区域在不同进程之间的共享方式，以及区域是否同步到相应的文件等等（具体参见man）。



munmap

取消address指定地址范围的映射。以后再引用取消的映射的时候就会导致非法内存的访问。这里address应该是页面的整数倍。

成功的时候这个函数返回0。

失败的时候，两者都返回-1.
