## **IIO Framework**（Industrial I/O）

**工业I / O（IIO）**是专用于模数转换器（ADC）和数模转换器（DAC）的内核子系统。随着越来越多的具有不同代码实现的传感器（具有模拟到数字或数字到模拟，功能的测量设备）分散在内核源上，收集它们变得必要。这就是IIO框架以通用的方式所做的事情。自2009年以来，Jonathan Cameron和Linux-IIO社区一直在开发它。

```
https://www.cnblogs.com/yongleili717/p/10744252.html
https://www.21ic.com/article/833583.html
```

以前的写法，就是驱动人员自己定义传感器数据的上报格式，自己定义的格式，那么久只有自己知道，自己编写应用。去诶单就是不灵活、封闭。没有一个标准，不统一。

理想的驱动：

黑匣子，应用通过统一模型直接读取到、人性化。

### iio_dev

devm_iio_device_alloc

iio_device_alloc 

​	 iio_priv

​		iio_device_register

- modes
- name
- channel

​	表示传感器的每一个测量通道

- info
  - read_raw
  - write_raw 
