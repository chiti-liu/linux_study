### 硬件实现

#### 热插拔

![image-20220816144313750](C:\Users\liujun\AppData\Roaming\Typora\typora-user-images\image-20220816144313750.png)

#### 集线器

可以在根集线器下再接一个集线器拓展，不过不能无限拓展。

### 主控制器驱动功能

1. 解析和维护URB（USB设备请求块）
2. 负责不同USB传输类型的调度工作
3. 负责USB数据的实际传输工作
4. 实现虚拟根HUB（根集线器）的功能

![image-20220816144800655](C:\Users\liujun\AppData\Roaming\Typora\typora-user-images\image-20220816144800655.png)

硬件存在为嵌入在SOC中，所以一般主控制器驱动不需要我们写

OHCI | UHCI | EHCI

drivers/usb/host

### USB设备驱动

![image-20220816151326257](C:\Users\liujun\AppData\Roaming\Typora\typora-user-images\image-20220816151326257.png)

#### 描述符（lsusb -v）

1. 设备描述符：struct usb_device_descriptor（厂家信息）

   ![image-20220816150431489](C:\Users\liujun\AppData\Roaming\Typora\typora-user-images\image-20220816150431489.png)

2. 配置描述符：struct usb_config_descriptor（寄存器）

   ![image-20220816150524012](C:\Users\liujun\AppData\Roaming\Typora\typora-user-images\image-20220816150524012.png)

3. 接口描述符：struct usb_interface_descriptor(驱动绑定在接口而不是设备上)

   ![image-20220816150720860](C:\Users\liujun\AppData\Roaming\Typora\typora-user-images\image-20220816150720860.png)

4. 端点描述符：struct usb_endpoint_descriptor（通讯细节）

   ![image-20220816150930805](C:\Users\liujun\AppData\Roaming\Typora\typora-user-images\image-20220816150930805.png)

#### 端点 endpoint

usb通信最基本的形式

端点只能往一个方向传输数据，（端口0除外，需要建立连接时数据交换）

端点存在于usb设备端

主机和端点之间的数据传输是通过pipe管道

#### 设备传输方式

1. 控制传输--获取、配置设备
2. 等时传输：周期性、连续的传输，对时间要求性高，不太关心中间数据的正确性，应用在音视频处理等。
3. 批量传输：大容量传输，非周期性的，传输延时不特别限制
4. 中断传输--例如usb鼠标，通过bInterval主机轮询查询自己的设备，从设备不主动申报。

#### usb requst block （urb）

![image-20220816152727674](C:\Users\liujun\AppData\Roaming\Typora\typora-user-images\image-20220816152727674.png)

![image-20220816154010428](C:\Users\liujun\AppData\Roaming\Typora\typora-user-images\image-20220816154010428.png)

  ![image-20220816154516059](C:\Users\liujun\AppData\Roaming\Typora\typora-user-images\image-20220816154516059.png)

![image-20220816155035012](C:\Users\liujun\AppData\Roaming\Typora\typora-user-images\image-20220816155035012.png)

### 操作方法

module_usb_driver(struct usb_driver)

​	name

​	id_table

​	probe

***

probe

​	kmalloc		//#include <linux/slab.h>

​	(struct usb_device *)interface_to_usbdev(usb_interface *)	//通过probe传入的接口获取USB设备

​	//获取端点，并判断端点的类型和方向是否正确

​	usb_rcvintpipe(bEndpointAddress)		//正确则利用端点地址创建管道

​	usb_maxpacket		//获取端点最大允许的包大小

​		struct urb *usb_alloc_urb(int iso_packets, gfp_t mem_flags)	//给端点创建urb

​			usb_set_infdata	//保存USB设备到接口

​	cdev_init(cdev, file_operations *)

​		cdev_add

​			init_waitqueue_head	//初始化队列



file_operations.read

​	usb_fill_bulk_urb(	urb, usb_device, pipe, setup_packet, buf, length,  **complete_fn_callback**, context)	//	查阅代码，发现，setup_packet一般不填，指定回调函数complete_fn_callback

​		usb_submit_urb

​			interruptible_sleep_on	//等待回调函数唤醒队列



complete_fn_callback	//urb传输完成后，回调函数被调用

​	//在函数中获取传输的状态和实际传输的字节数

​		wake_up_interruptible	//唤醒队列

​	

​		

​	

​	