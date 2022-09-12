/*
 *	OLED驱动
 */


#include "oled_device.h"

  
  
#define  DEVICE_COUNT	 	1 		/* 设备号个数 */
#define  DEVICE_NAME	    "oled_device" 	/* 设备名字 */
  
/* 设备结构体 */
struct  device_struct
{
  dev_t devid; 					 /* 设备ID */
  struct cdev cdev; 		  /* 字符设备 */
  struct class *class; 			/* 类 */
  struct device *device; 	/* 设备 */
  int major; 						/* 主设备号 */
  struct device_node *nd; /* 设备节点 */
  int  oled_clk_gpio; 			/* oled 所使用的 GPIO 编号 */
	int  oled_din_gpio; 
	int  oled_res_gpio; 
	int  oled_dc_gpio; 
	int  oled_cs_gpio; 
};
  
struct  device_struct  oled; 	/* oled 设备 */


//向SSD1106写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(uint8_t dat,uint8_t cmd)
{	
	uint8_t i;	
	switch(cmd)
	{
		case OLED_CMD:
		{
			OLED_DC_Low();	
			OLED_CS_Low();
			for(i=0;i<8;i++)
			{			  
				OLED_SCLK_Low();
				if(dat&0x80)
		   			OLED_SDIN_High();
				else 
		   			OLED_SDIN_Low();
				OLED_SCLK_High();
				dat<<=1;   
			}				 		  
			OLED_CS_High();
			OLED_DC_High();   
		}
			break;
		case OLED_DATA:
		{
			OLED_DC_High();	
			OLED_CS_Low();
			for(i=0;i<8;i++)
			{			  
				OLED_SCLK_Low();
				if(dat&0x80)
		   			OLED_SDIN_High();
				else 
		   			OLED_SDIN_Low();
				OLED_SCLK_High();
				dat<<=1;   
			}				 		  
			OLED_CS_High();
			OLED_DC_High();   
		}
			break;
		case OLED_SCLK_LOW:
			OLED_SCLK_Low();	
			break;
		case OLED_SCLK_HIGH:
			OLED_SCLK_High();	
			break;
		case OLED_SDIN_LOW:
			OLED_SDIN_Low();	
			break;
		case OLED_SDIN_HIGH:
			OLED_SDIN_High();	
			break;
		case OLED_RES_LOW:
			OLED_RES_Low();	
			break;
		case OLED_RES_HIGH:
			OLED_RES_High();	
			break;
		case OLED_DC_LOW:
			OLED_DC_Low();	
			break;
		case OLED_DC_HIGH:
			OLED_DC_High();	
			break;
		case OLED_CS_LOW:
			OLED_CS_Low();	
			break;
		case OLED_CS_HIGH:
			OLED_CS_High();	
			break;
		default:
			break;
	}		  
} 

  
/*
 * @description : 打开设备
 * @param – inode : 传递给驱动的 inode
 * @param – filp : 设备文件，file 结构体有个叫做 private_data 的成员变量
 * 一般在 open 的时候将 private_data 指向设备结构体。
 * @return : 0 成功;其他 失败
 */
static int device_open(struct inode *inode, struct file *filp)
{
  filp->private_data = &oled; /* 打开设备 */
  return 0;
}
  
/*
 * @description : 从设备读取数据
 * @param – filp : 要打开的设备文件(文件描述符)
 * @param - buf : 返回给用户空间的数据缓冲区
 * @param - cnt : 要读取的数据长度
 * @param – offt : 相对于文件首地址的偏移
 * @return : 读取的字节数，如果为负值，表示读取失败
 */
static ssize_t device_read(struct file *filp, char __user *buf,size_t cnt, loff_t *offt)
{
  return 0;
}
  
/*
 * @description : 向设备写数据
 * @param - filp : 设备文件，表示打开的文件描述符
 * @param - buf : 要写给设备写入的数据
 * @param - cnt : 要写入的数据长度
 * @param – offt : 相对于文件首地址的偏移
 * @return : 写入的字节数，如果为负值，表示写入失败
 */
static ssize_t device_write(struct file *filp, const char __user *buf,size_t cnt, loff_t *offt)
{
	int retval;
	unsigned char databuf[2];
	uint8_t  data, cmd;
  	
	retval = copy_from_user(databuf, buf, cnt);
  if(retval < 0)
	{
  	printk("kernel write failed!\r\n");
  	return -EFAULT;
	}

	data = databuf[0];
	cmd = databuf[1];

  OLED_WR_Byte(data, cmd);	 
  return 0;
}
  
/*
 * @description : 关闭/释放设备
 * @param – filp : 要关闭的设备文件(文件描述符)
 * @return : 0 成功;其他 失败
 */
static int device_release(struct inode *inode, struct file *filp)
{
	return 0;
}
  
/* 设备操作函数 */
static struct file_operations device_fops = 
{
	.owner = THIS_MODULE,
	.open = device_open,
	.read = device_read,
	.write = device_write,
	.release = device_release,
};
  
/*
 * @description : 驱动入口函数
 * @param : 无
 * @return : 无
 */
static int device_probe(struct platform_device *dev)
{
	int retval = 0;
	/* 在设备树中寻找OLED节点 */
  oled.nd = of_find_node_by_path("/oled_device");
  if(oled.nd == NULL) 
	{
  	printk("OLED node cant not found!\r\n");
  	return -EINVAL;
  } 
	else 
	{
  	printk("OLED node has been found!\r\n");
  }
  oled.oled_clk_gpio = of_get_named_gpio(oled.nd, "oled-clk-gpio", 0);
	if(oled.oled_clk_gpio < 0) 
	{
  	printk("can't get oled-clk-gpio");
  	return -EINVAL;
  }
  retval = gpio_direction_output(oled.oled_clk_gpio, 1);
  if(retval < 0) 
	{
  	printk("can't set oled-clk-gpio!\r\n");
  }

  oled.oled_din_gpio = of_get_named_gpio(oled.nd, "oled-din-gpio", 0);
  if(oled.oled_din_gpio < 0) 
	{
  	printk("can't get oled-din-gpio");
  	return -EINVAL;
  }
  retval = gpio_direction_output(oled.oled_din_gpio, 1);
  if(retval < 0) 
	{
  	printk("can't set oled-din-gpio!\r\n");
  }

	oled.oled_res_gpio = of_get_named_gpio(oled.nd, "oled-res-gpio", 0);
  if(oled.oled_res_gpio < 0) 
	{
  	printk("can't get oled-res-gpio");
  	return -EINVAL;
  }
  retval = gpio_direction_output(oled.oled_res_gpio, 1);
  if(retval < 0) 
	{
  	printk("can't set oled-res-gpio!\r\n");
  }

  oled.oled_dc_gpio = of_get_named_gpio(oled.nd, "oled-dc-gpio", 0);
	if(oled.oled_dc_gpio < 0) 
	{
		printk("can't get oled-dc-gpio");
		return -EINVAL;
	}
  retval = gpio_direction_output(oled.oled_dc_gpio, 1);
  if(retval < 0) 
	{
  	printk("can't set oled-dc-gpio!\r\n");
	}

	oled.oled_cs_gpio = of_get_named_gpio(oled.nd, "oled-cs-gpio", 0);
	if(oled.oled_cs_gpio < 0) 
	{
  	printk("can't get oled-cs-gpio");
  	return -EINVAL;
  }
  retval = gpio_direction_output(oled.oled_cs_gpio, 1);
  if(retval < 0) 
	{
  	printk("can't set oled-cs-gpio!\r\n");
  }

	/* 初始化设为高电平 */
	OLED_SCLK_High();
	OLED_SDIN_High();
	OLED_RES_High();
	OLED_DC_High();
	OLED_CS_High();

  
  /* 注册字符设备驱动 */
  /* 1、创建设备号 */
  alloc_chrdev_region(&oled.devid, 0, DEVICE_COUNT, DEVICE_NAME); /* 申请设备号 */
  oled.major = MAJOR(oled.devid); /* 获取分配号的主设备号 */
  
  /* 2、初始化字符设备 */
  oled.cdev.owner = THIS_MODULE;
  cdev_init(&oled.cdev, &device_fops);
  
  /* 3、添加一个字符设备 */
  cdev_add(&oled.cdev, oled.devid, DEVICE_COUNT);
  
  /* 4、创建类 */
  oled.class = class_create(THIS_MODULE, DEVICE_NAME);
  if (IS_ERR(oled.class)) 
	{
  	return PTR_ERR(oled.class);
  }
  
  /* 5、创建设备 */
  oled.device = device_create(oled.class, NULL, oled.devid, NULL, DEVICE_NAME);
	if (IS_ERR(oled.device)) 
	{
  	return PTR_ERR(oled.device);
	}
  return 0;
}
  
/*
 * @description : 驱动出口函数
 * @param : 无
 * @return : 无
 */
static int device_remove(struct platform_device *dev)
{
	OLED_SCLK_Low();	
	OLED_SDIN_Low();
	OLED_RES_Low();
	OLED_DC_Low();
	OLED_CS_Low();

  /* 注销字符设备驱动 */
  cdev_del(&oled.cdev);
  unregister_chrdev_region(oled.devid, DEVICE_COUNT);
  
  device_destroy(oled.class, oled.devid);
  class_destroy(oled.class);

	return  0;
}


/* 匹配列表 */
static const struct of_device_id  device_imx_dt_ids[] = {
	{ .compatible = "oled-gpios" },
	{ /* Sentinel */ }
};

/* platform驱动结构体 */
static struct platform_driver device_driver = 
{
	.probe		  = device_probe,
	.remove		= device_remove,
	.driver		   = 
	{
		.name	= DEVICE_NAME,		
		.of_match_table	= device_imx_dt_ids, 
	},
};

module_platform_driver(device_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("dazen");

