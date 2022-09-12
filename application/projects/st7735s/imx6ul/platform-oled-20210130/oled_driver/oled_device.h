#ifndef __OLED_DEVICE_H
#define __OLED_DEVICE_H	

#include <linux/types.h> 
#include <linux/kernel.h> 
#include <linux/delay.h> 
#include <linux/ide.h> 
#include <linux/init.h> 
#include <linux/module.h> 
#include <linux/errno.h> 
#include <linux/gpio.h> 
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>


#define	uint8_t    unsigned char
#define	uint16_t  unsigned int
#define	uint32_t  unsigned long


extern  struct  device_struct  oled; 	/* oled 设备 */


#define OLED_CMD  0	// 写命令
#define OLED_DATA 1	// 写数据

#define OLED_SCLK_LOW   10    // OLED_SCLK_LOW
#define OLED_SCLK_HIGH  11    // OLED_SCLK_HIGH
#define OLED_SDIN_LOW    12    // OLED_SDIN_LOW
#define OLED_SDIN_HIGH  13    // OLED_SDIN_HIGH
#define OLED_RES_LOW     14    // OLED_RES_LOW
#define OLED_RES_HIGH    15    // OLED_RES_HIGH
#define OLED_DC_LOW      16    // OLED_DC_LOW
#define OLED_DC_HIGH      17    // OLED_DC_HIGH
#define OLED_CS_LOW       18    // OLED_CS_LOW
#define OLED_CS_HIGH       19    // OLED_CS_HIGH



/* -----------------OLED端口定义---------------- */
#define OLED_SCLK_Low()   gpio_set_value(oled.oled_clk_gpio, 0)//SCLK
#define OLED_SCLK_High()   gpio_set_value(oled.oled_clk_gpio, 1)

#define OLED_SDIN_Low()   gpio_set_value(oled.oled_din_gpio, 0)//SDIN(MOSI)
#define OLED_SDIN_High()   gpio_set_value(oled.oled_din_gpio, 1)

#define OLED_RES_Low()   gpio_set_value(oled.oled_res_gpio, 0)//RES
#define OLED_RES_High()   gpio_set_value(oled.oled_res_gpio, 1)

#define OLED_DC_Low()    gpio_set_value(oled.oled_dc_gpio, 0)//DC
#define OLED_DC_High()    gpio_set_value(oled.oled_dc_gpio, 1)
 		     
#define OLED_CS_Low()    gpio_set_value(oled.oled_cs_gpio, 0)//CS
#define OLED_CS_High()    gpio_set_value(oled.oled_cs_gpio, 1)

void OLED_WR_Byte(uint8_t dat,uint8_t cmd);	   //OLED控制用函数

#endif  

