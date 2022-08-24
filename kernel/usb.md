### 硬件实现

#### 热插拔

![image-20220816144313750](..\typora-user-images\image-20220816144313750.png)

#### 集线器

可以在根集线器下再接一个集线器拓展，不过不能无限拓展。

### 主控制器驱动功能

1. 解析和维护URB（USB设备请求块）
2. 负责不同USB传输类型的调度工作
3. 负责USB数据的实际传输工作
4. 实现虚拟根HUB（根集线器）的功能

![image-20220816144800655](..\typora-user-images\image-20220816144800655.png)

硬件存在为嵌入在SOC中，所以一般主控制器驱动不需要我们写

OHCI | UHCI | EHCI

drivers/usb/host

### USB设备驱动

![image-20220816151326257](..\typora-user-images\image-20220816151326257.png)

#### 描述符（lsusb -v）

1. 设备描述符：struct usb_device_descriptor（厂家信息）

   ![image-20220816150431489](..\typora-user-images\image-20220816150431489.png)

2. 配置描述符：struct usb_config_descriptor（寄存器）

   ![image-20220816150524012](..\typora-user-images\image-20220816150524012.png)

3. 接口描述符：struct usb_interface_descriptor(驱动绑定在接口而不是设备上)

   ![image-20220816150720860](..\typora-user-images\image-20220816150720860.png)

4. 端点描述符：struct usb_endpoint_descriptor（通讯细节）

   ![image-20220816150930805](..\typora-user-images\image-20220816150930805.png)

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

![image-20220816152727674](..\typora-user-images\image-20220816152727674.png)

![image-20220816154010428](..\typora-user-images\image-20220816154010428.png)

  ![image-20220816154516059](..\typora-user-images\image-20220816154516059.png)

![image-20220816155035012](..\typora-user-images\image-20220816155035012.png)

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

### 框架分析

```
https://www.cnblogs.com/liusiluandzhangkun/p/8595286.html
```

 首先我们先来简单说一说USB的框架，之后在来具体分析源码，以便加深理解！其实USB的框架比较像“平台总线、设备、驱动”的框架，也分为总线、设备、驱动三大块。其中总线驱动是已经由内核完成的，一旦接入usb设备，总线驱动程序就会找到能处理该设备的驱动进行处理！我们进入文件系统的/sys/bus目录下可以看到很多总线目录，usb目录就在其中，我们进入usb目录，会看到devices和drivers！

[![linux usb](http://www.techbulo.com/wp-content/uploads/2014/09/linux-usb-300x160.jpg)](http://www.techbulo.com/wp-content/uploads/2014/09/linux-usb.jpg)



下面我们来看源码：

\#define subsys_initcall(fn) module_init(fn)

subsys_initcall(usb_init);

   bus_register(&usb_bus_type);1

​      bus_register(subsys, &__key);

​         retval = kset_register(&priv->subsys);

​           kobject_add_internal(&k->kobj);

​              kobj_kset_join(kobj);

​                 //将usb总线加入到总线链表里面，sys/bus目录下就是所有的总线目录

​                 list_add_tail(&kobj->entry, &kobj->kset->list);

​              create_dir(kobj);

​                 sysfs_create_dir(kobj);//具体的目录在这里创建

​        //在总线下创建了devices目录，据我分析这里并没有添加设备链表，因为还没有去发现设备

​        priv->devices_kset = kset_create_and_add("devices", NULL,&priv->subsys.kobj);

​        //在总线下创建了drivers目录，据我分析这里并没有添加驱动链表，因为还没有去注册驱动

​        klist_init(&priv->klist_devices, klist_devices_get, klist_devices_put); **//初始化设备链表**

​        klist_init(&priv->klist_drivers, NULL, NULL); **//初始化驱动链表**

   **retval = usb_major_init();//这里面注册了一个字符设备**

​     **//#define USB_MAJOR  180，cat /proc/devices可以观察到**

​     register_chrdev(USB_MAJOR, "usb", &usb_fops);

   **retval = usb_hub_init();**

​      usb_register(&hub_driver)**//将hub.c里面的****hub_driver加入到usb总线下的驱动链表里**

​        usb_register_driver(driver, THIS_MODULE, KBUILD_MODNAME)

​           driver_register(&new_driver->drvwrap.driver);

​              bus_add_driver(drv);

​                 driver_attach(drv);

​                    **//匹配devices与drivers，调用****__driver_attach**

​                    bus_for_each_dev(drv->bus, NULL, drv, __driver_attach);

​                        __driver_attach

​                           driver_match_device(drv, dev)

​                               **//调用总线上的match，检测驱动与设备是否匹配**

​                               drv->bus->match ? drv->bus->match(dev, drv) : 1;

​                            driver_probe_device(drv, dev);

​                               really_probe(dev, drv);

​                                   drv->probe(dev); **//调用hub.c里面的驱动的probe函数，这里的drv是device_driver结构体，其在usb_register_driver过程中通过new_driver->drvwrap.driver.probe = usb_probe_interface;来设置，这个probe会调用driver中的probe**

​                                      hub_probe

​                                         hub_configure(hub, endpoint)

​                                             **//这里注册了中断，一旦接入新的usb设备就会调用****hub_irq**

​                                             usb_fill_int_urb(hub->urb, hdev, pipe, *hub->buffer, maxp,

​                                                  hub_irq,hub, endpoint->bInterval);

​      khubd_task = kthread_run(hub_thread, NULL, "khubd"); **//启动****hub_thread这个线程**

​         hub_thread

​            **//#define wait_event_freezable(wq, condition)  wait_event_interruptible(wq, condition)**

​            **//将当前进程加入到等待队列中，进程在这里停下来了，我们需要看看那里唤醒进程**

​            wait_event_freezable(khubd_wait,!list_empty(&hub_event_list) ||kthread_should_sto());

​            **//一旦接入新的usb设备，就会调用****hub_irq**

​               hub_irq

​                  kick_khubd(hub);

​                     wake_up(&khubd_wait);**//唤醒等待队列里面的进程，执行hub_events();**

​               hub_events();

​                 hub_port_connect_change(hub, i,portstatus, portchange);

​                    hub_port_init(hub, udev, port1, i);

​                       udev = usb_alloc_dev(hdev, hdev->bus, port1);

​                       choose_devnum(udev);**//选择usb设备地址**

​                          hub_set_address(udev, devnum);**//将选择的地址告诉usb设备**

​                          usb_get_device_descriptor(udev, 8);**//获得设备描述符**

​                          usb_get_device_descriptor(udev, USB_DT_DEVICE_SIZE);**//再次获得设备描述符**

​                    usb_new_device(udev);

​                        usb_enumerate_device(udev);

​                            usb_get_configuration(udev);**//获得配置信息**

​                            **//获得产品信息**

​                            udev->product = usb_cache_string(udev, udev->descriptor.iProduct);

​                            **//获得制造商信息**

​                            udev->manufacturer = usb_cache_string(udev,udev->descriptor.iManufacturer);

​                            **//获得序列号信息**

​                            udev->serial = usb_cache_string(udev, udev->descriptor.iSerialNumber);

​                            device_add(&udev->dev);

​                              bus_add_device(dev);**//将usb设备加入到usb总线旗下的设备列表里面**

​                              bus_probe_device(dev);

​                                  device_attach(dev);

​                                      **//对所有的驱动，调用****__device_attach判断设备与驱动是否匹配**

​                                     bus_for_each_drv(dev->bus, NULL, dev, __device_attach);

​                                         driver_match_device(drv, dev)

​                                             **//调用总线下的match函数来判断设备与驱动时候是否匹配**

​                                             drv->bus->match ? drv->bus->match(dev, drv) : 1;

​                                         driver_probe_device(drv, dev);

​                                             really_probe(dev, drv);

　　　　　　　　　　　　　　　　　　　　　　　　　　　 //说明：**//调用hub.c里面的驱动的probe函数，这里的drv是device_driver结构体，其在usb_register_driver过程中通过new_driver->drvwrap.driver.probe = usb_probe_interface;来设置，这个probe会调用driver中的probe**

​                                                \#drv->probe(dev);**//一旦匹配的话就会调用驱动的probe函数（有问题）**

   **//将generic****.c里面的 usb_generic_driver 加入到usb总线下的驱动链表里**

   **retval = usb_register_device_driver(&usb_generic_driver, THIS_MODULE);**

 

**下面我们来看看usb总线的match函数：**

usb_device_match

​    usb_match_id(intf, usb_drv->id_table)

​        usb_match_one_id(interface, id)

​           usb_match_device(dev, id)

根据驱动id和设备信息是否匹配来判断是否匹配

 

关于判断匹配，上面的分析中涉及到了两次，第一次匹配时hub.c里面定义的驱动与匹配，第二次是我们自己定义的驱动匹配具体的设备！

第一次hub中也是调用usb_register来注册，发现hub设备匹配id_table时调用hub的probe函数，第二次是设备和我们的驱动

 

由此我们的usb框架就很清楚了，虽然是分为三大块，但是涉及具体的驱动，我们可以分为两层：usb总线驱动层和usb设备驱动层。系统初始化过程中会使usb进程进入休眠，一旦接入usb设备，就会唤醒进程，获取设备的相关信息，然后遍历驱动链表，调用总线的match函数来寻找与设备相匹配的驱动，一旦找到，就会调用驱动的porbe函数！框架很简单，我们写usb驱动，要做的就是写usb设备驱动，而主要的工作就在probe函数里面完成！

​		

​	

​	