# <<linux 驱动开发>>学习日志

```
MODULE_LICENSE（“GPL”）//申明采用了开源协议，可以使用一些开源GPL代码API
_init/_exit   在加载前占据内存，加载完后自动销毁留出空间
obj-m:模块，生成ko文件，但需要手动加载
obj-y:直接.o编进内核
objs:多.o文件编译成模块

```

#### 疑问：all和modules在Makefile中的区别?

答：没什么区别，就是别名而已，make默认执行第一个目标。

内核模块可以设置参数，在加载时进行控制  例如串口模块可以设置波特率、端口、名字等等

```
EXPORT_SYMBOL	//内部对变量或者函数可以让该函数或变量可以被其他模块使用，使用前用extern申明

EXPORT_SYMBOL_GPL	//内部对变量或者函数可以让该函数或变量可以被其他模块使用（申明了GPL协议）的使用
```

#### 疑问：static不是可以限制函数或者变量只可以在当前文件中吗？为什么依然可以EXPORT_SYMBOL申明为外部，那这样申明有意义吗？只为了在某个文件中使用？

答：Linux-2.6之后默认不导出所有的符号，所以使用EXPORT_SYMBOL() 做标记。static增加可读性。EXPORT_SYMBOL标签内定义的函数或者符号对全部内核代码公开，不用修改内核代码就可以在内核模块中直接调用，
即使用EXPORT_SYMBOL可以将一个函数以符号的方式导出给其他模块使用。
符号的意思就是函数的入口地址，或者说是把这些符号和对应的地址保存起来的，在内核运行的过程中，可以找到这些符号对应的地址的。

EXPORT_SYMBOL使用方法
        1. 在模块函数定义之后使用EXPORT_SYMBOL（函数名）
                2.在调用该函数的模块中使用extern对要使用的符号或者函数进行声明
                  3.首先加载定义该函数的模块，再加载调用该函数的模块

```
modprobe优于insmod: 自动加载被依赖的模块，insmod要注意自己模块加载的先后可能导致的问题。

depmod: 生成模块的依赖信息。

modinfo: 生成模块的具体版本信息
```

**另外，如果当前模块板块与内核版本不一致，那么模块将会被拒绝载入。**

卸载模块时也要注意，可能一个模块在使用另一个模块的函数，需要先把当前模块卸载再卸载另一个模块。

#### 内核模块和应用程序之间的差异：

- 内核模块是操作系统内核的一部分，运行在内核空间；应用程序在用户空间。
- 内核模块被动调用，只在初始化和清除函数分别在被加载和被卸载时调用；应用程序则是顺序执行，或者在某一循环反复调用。
- 内核函数在C库函数之下，不能调用；应用则在之上，可以调用。
- 内核模块通常要做一些清除性质的工作，通常在加载失败或者内核的清除函数中。
- 内核模块如果发生非法访问，则整个系统可能都会崩溃。应用程序只会影响自己。
- 内核的并发有很多，如中断、多处理器等；应用程序一般只考虑多线程或者多进程。
- 内核空间一般只有4KB、8KB的栈，如果需要更大的内存空间，通常需要动态申请分配。
- printk不能打印浮点类型，否则编译时会出警告，并且模块加载不会成功。

### 中断

可以分为上半部和下半部

上半部：硬件中断，处理一些紧急且快速的事情；

下半部：软件中断，处理一些不是那么紧急或者慢的事情；

软件中断可以被硬件中断打断；

软件中断技术：

事情不是太长：tasklet

事情多且复杂：工作队列

新技术：thread_irq(针对多核设备，工作队列和tasklet只能在单核运行，浪费CPU资源)

（hardwareirq,irq）同时注册在domain域中

![image-20220804111804080](..\typora-user-images\image-20220804111804080.png)

#### 疑问：如果一个硬件中断同时对应比如按键中断和外部输入中断，那么究竟是怎么详细区分的呢？

答：载入每一个irq去执行确定到底是哪一个（**但是概念很模糊啊，还是没搞清具体怎么判断**）。

#### 异步通知

![image-20220804160358327](..\typora-user-images\image-20220804160358327.png)

### kzalloc/kmalloc/vmalloc

```
https://www.cnblogs.com/sky-heaven/p/7390370.html
```

用kzalloc申请内存的时候， 效果等同于先是用 *kmalloc()* 申请空间 *,* 然后用 *memset()* 来初始化 *,*所有申请的元素都被初始化为 *0.*

kmalloc()、kzalloc()、vmalloc() 的共同特点是：

1. 用于申请内核空间的内存；
2. 内存以字节为单位进行分配；
3. 所分配的内存虚拟地址上连续；

kmalloc()、kzalloc()、vmalloc() 的区别是：

1. kzalloc 是强制清零的 kmalloc 操作；（以下描述不区分 kmalloc 和 kzalloc）
2. kmalloc 分配的内存大小有限制（128KB），而 vmalloc 没有限制；
3. kmalloc 可以保证分配的内存物理地址是连续的，但是 vmalloc 不能保证；
4. kmalloc 分配内存的过程可以是原子过程（使用 GFP_ATOMIC），而 vmalloc 分配内存时则可能产生阻塞；
5. kmalloc 分配内存的开销小，因此 kmalloc 比 vmalloc 要快；

一般情况下，内存只有在要被 DMA 访问的时候才需要物理上连续，但为了性能上的考虑，内核中一般使用 kmalloc()，而只有在需要获得大块内存时才使用 vmalloc()。例如，当模块被动态加载到内核当中时，就把模块装载到由 vmalloc() 分配的内存上。

### fcntl/ioctl

```
http://blog.chinaunix.net/uid-21651676-id-60392.html
https://www.cnblogs.com/kelamoyujuzhen/p/9688307.html
```



### 自动加载&手动加载

设备插入主板后自动加载驱动，可以用udev/mdev，推荐用udev。

使用modprobe前需要把设备驱动移入/lib/modules/$(uname -r)/ 中，并且depmod更行依赖。

### container_of

```
https://www.linuxidc.com/Linux/2012-02/53701.htm#:~:text=containe,%E9%87%8F%E7%9A%84%E6%8C%87%E9%92%88%E7%9A%84%E5%8A%9F%E8%83%BD%E3%80%82
```

根据数据成员的地址确定包含该数据成员结构体的地址

### 驱动编写步骤

1. 看sch:怎么操作
2. 看芯片手册
3. 规划驱动功能
4. 写驱动、测试程序 

​        首先查看驱动 kernel/drivers/**目录里面有没有编写对应的驱动，有必要可以参照对应的LRM手册。

- 如果驱动已经实现，则make ARCH=arm menuconfig配置内核，配置好后保存，执行make ARCH=arm uImage 然后拷贝到tftpboot中。然后参照Documentation/devicetree/bindings/**中的文件，修改设备树节点，compatible属性要和驱动相互匹配，保存好执行make ARCH=arm dtbs，并且拷贝到tftboot中，重启开发板
- 如果驱动没有实现，则需要去通用设备树找到相关节点，因为一般厂家都把对应的节点初始化好了，或者自己加入子节点，注意在这之气那要看寄存器配置和手册，不过要看父节点定义，然后在板级设备树中使能相应节点，重新编译，再重新编写驱动，注意看会不会和原来的冲突。

### kset/kobject/ktype

```
https://www.cnblogs.com/xiaojiang1025/p/6193959.html
```

​        **kset**表示一组**kobject**的集合，**kobject**通过**kset**组织成层次化的结构，所有属于该**kset**的**kobject**结构的**parent**指针指向**kset**包含的**kobject**对象，构成一个父子层次关系这些**kobject**可以是不同或相同的类型(kobj_type)。

​        sysfs中的设备组织结构很大程度上都是根据**kset**进行组织的，比如**"/sys/drivers"**目录就是一个**kset**对象，包含系统中的驱动程序对应的目录，驱动程序的目录又kobject表示。

​        比如在平台设备模型中，当我们注册一个设备或驱动到平台总线，其实是将对应的**kobject**挂接到platform总线的**kset**上，每种总线都是维护两条链表(两个kset)，一条用于链接挂接在上面的驱动(驱动kset)，一条用于链接挂接在上面的设备(设备kset)。

- kset就是管理节点，负责挂靠kobject
- kobject负责挂靠设备和驱动节点
- ktype表示当前kobject的类型
- 都是设备管理中的基本结构体

​        其中kset里面有个：

​        **list_head**还是那个用来挂在链表上的结构，包含在一个**kset**的所有kobject构成了一个**双向循环链表**，**list_head**就是这个链表的头部，这个链表用来连接第一个和最后一个kobject对象，第一个kobject使用entry连接kset集合以及第二个kobject对象，第二个kobject对象使用entry连接第一个kobject对象和第三个kobject对象，依次类推，最终形成一个kobject对象的链表。

![image-20220817103056518](..\typora-user-images\image-20220817103056518.png)

### 互斥和同步

1. 为什么自旋锁的临界区不能睡眠？
2. arm 64处理器当中，我们如何实现独占访问内存。
3. 排队自旋锁是如何实现MCS锁的？
4. 乐观自旋锁等待的判断条件是什么？
5. 请你说出MCS锁机制的实现原理？



临界区（critical region）是指访问和操作共享数据的代码段，其中的资源无法同时被所得执行线程访问，访问临界区的执行线程或代码路径成为并发源。我们为了避免并发访问临界区，软件工程师必须保证访问临界区的原子性，即在临界区被不能有多个并发源同时执行，整个临界区就像一个不可分割的整体。

在linux内核当中产生访问呢的并发源主要有：中断和异常、内核抢占、多处理器并发执行、软中断和tasklet。

考虑SMP（Share memory processor）系统：

- 同一类型的中断程序不会并发执行，但是不同类型的中断可能送达可能送达不同的CPU，因此不同类型的中断处理程序可能会并发执行。
- 同一类型的软中断会在不同的CPU上并发执行。
- 同意类型的tasklet是串行执行的，不会在多个PCU上并发执行。
- 不同CPU上的进程上下文会并发执行。

![image-20220822141409545](../typora-user-images/image-20220822141409545.png)

#### 中断屏蔽

#### 原子变量

原子操作保证指令以原子的反射光hi执行，执行过程中不会被打断。假设两个线程thread_afunc和thread_bfunc执行i++操作，那么i最后等于？

![image-20220822142119542](../typora-user-images/image-20220822142119542.png)



#### 自旋锁

自旋锁一直占据线程不阻塞。自旋锁时间必须短且不可以引入进程切换，否则可能死锁。

#### 读写锁

#### 顺序锁

#### 信号量

#### 互斥量

#### RCU机制

### 驱动检测设备原理

#### 不支持热插拔设备

系统启动或者加载.ko文件时检测对应端口，如果有对应ID则加载、初始化并且添加到对应的/dev设备文件中

#### 支持热插拔设备

支持热插拔的设备分为两种

1. 单独的引脚检测是否有设备插入
2. 有对应接口一直检测设备

### 疑问：根目录下/sys、/proc和/dev文件夹有什么区别？

/dev表示已经挂载的设备

/proc表示正在运行的内存信息映射

/sys硬件设备的驱动程序信息，其中/sys/devices/platform里面有基于平台的所有驱动

### 内联汇编

![image-20220822150643362](../typora-user-images/image-20220822150643362.png)

### 设备树和驱动匹配

platform_match

![image-20220822171342061](../typora-user-images/image-20220822171342061.png)

platform_driver

​	.driver

​		of_match_table

​			of_device_id

​				.compatible

​	(或者).id_table

### struct inode 和struct file

```
https://www.cnblogs.com/QJohnson/archive/2011/06/24/2089414.html
```

### 设备树节点匹配platform_deivce和驱动

```
https://blog.csdn.net/Aa_lihua/article/details/106064773
```

### &gpio*

![image-20220824100806571](../typora-user-images/image-20220824100806571.png)

dtc -I dtb -O dts *** .dtb > ***.dts反汇编

![image-20220824100730798](../typora-user-images/image-20220824100730798.png)

### 节点构造函数解析

**注意：节点有关文件与操作，是重点**

```
https://deepinout.com/linux-kernel-api/device-driver-and-device-management/
```

#### register_chrdev(major,name"/proc/devices",file oerations *)

函数**register_chrdev()**调用函数__register_chrdev()实现其功能，函数__register_chrdev()首先调用函数__register_chrdev_region()创建一个字符设备区，此设备区的主设备号相同，由函数register_chrdev()的第一个参数决定，次设备号的变化范围是0～256，设备区的名字为函数register_chrdev()的第二个参数，此函数将更改`/proc/devices`文件的内容；然后动态申请一个新的字符设备cdev结构体变量，对其部分字段进行初始化，初始化完成之后将其加入Linux内核系统中，即向Linux内核系统添加一个新的字符设备。函数register_chrdev()调用函数cdev_alloc()动态申请一个字符设备，调用函数cdev_add()将其加入Linux内核系统中。

- 文件包含：#include <linux/fs.h>
- int __register_chrdev(unsigned int major, unsigned int baseminor,
  		      unsigned int count, const char *name,
    		      const struct file_operations *fops)
  - __register_chrdev_region(major, baseminor, count, name);
  - cdev_alloc();
  - kobject_set_name(&cdev->kobj, "%s", name);
  - cdev_add(cdev, MKDEV(cd->major, baseminor), count);

#### device_create(class,    ,MKDEV(),    ,name""/dev/name"")

函数**device_create()**用于动态地创建逻辑设备，并对新的逻辑设备类进行相应的初始化，将其与此函数的第一个参数所代表的逻辑类关联起来，然后将此逻辑设备加到Linux内核系统的设备驱动程序模型中。函数能够自动地在`/sys/devices/virtual`目录下创建新的逻辑设备目录，在`/dev`目录下创建与逻辑类对应的设备文件。

- 文件包含：#include <linux/device.h>

- 定义

  ```
  struct device *device_create(struct class *cls, struct device *parent, dev_t devt, void *drvdata, const char *fmt, ...);
  ```

  

#### class_create(MODULE,name"/sys/class/name")

宏**class_create()**用于动态创建设备的逻辑类，并完成部分字段的初始化，然后将其添加进Linux内核系统中。此函数的执行效果就是在目录`/sys/class`下创建一个新的文件夹，此文件夹的名字为此函数的第二个输入参数，但此文件夹是空的。宏class_create()在实现时，调用了函数[__class_create](https://deepinout.com/linux-kernel-api/device-driver-and-device-management/linux-kernel-api__class_create.html)()，作用和函数__class_create()基本相同。

- 文件包含：#include <linux/device.h>

- 宏定义：

  ```
  #define class_create(owner, name)		\
  ({						\
  	static struct lock_class_key __key;	\
  	__class_create(owner, name, &__key);	\
  })	
  ```

  - 参数`owner`是一个struct module结构体类型的指针，指向函数__class_create()即将创建的struct class类型对象的拥有者，一般赋值为THIS_MODULE，此结构体的详细定义见文件linux-3.19.3/include/linux/module.h。
  - 参数`name`是char类型的指针，代表即将创建的struct class变量的名字，用于给struct class的name字段赋值。

### u-boot rk3399加载设备树

```
https://blog.csdn.net/sements/article/details/104795430
```

需要知道的一点是，在arm64架构下，linux已经弃用mach_xx等文件夹来描述板级信息供linux内核启动时去匹配根节点下的compatible属性来找到对应设备树。

取而代之是单纯的接收bootloader传递过来的单一设备树文件dtb所在的内存地址。所以当多个dtb文件存在镜像中或者内存中时，如何找到需要的dtb的这个任务，就落在了bootloader肩上。


### make menuconfig

- Kconfig

- Makefile
- drv.c
