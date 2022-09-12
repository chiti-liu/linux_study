
#include "oled_api.h"
#include "oled_font.h"
#include "oled_bmp.h"



char *filename = "/dev/oled_device";


/* 
 * @向OLED设备写入一个字节数据
 * @data：写入的数据
 * @cmd：写入的是数据还是命令，0表示命令；1表示数据
 */
void OLED_Write_Data(uint8_t data, uint8_t cmd)
{
	int  fd;
	int  retvalue = 0;
	unsigned char databuf[2];

	fd = open(filename, O_RDWR);  /* 向OLED设备文件写数据 */
	if(fd < 0)
	{
		printf("OLED file open failed!\r\n");
		close(fd);
		while(1);
	}

	databuf[0] = data;
	databuf[1] = cmd;

	retvalue = write(fd, &databuf[0], 2);  /* 向OLED设备文件写数据 */
	if(retvalue < 0)
	{
		printf("OLED file write failed!\r\n");
		close(fd);
		while(1);
	}

	retvalue = close(fd);	/* 关闭OLED设备文件 */
	if(retvalue < 0)
	{
		printf("OLED file close failed!\r\n");
		while(1);
	}
}


//设置OLED起点 
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_Write_Data(0xb0+y,OLED_CMD);
	OLED_Write_Data(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_Write_Data((x&0x0f)|0x01,OLED_CMD); 
}   	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_Write_Data(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_Write_Data(0X14,OLED_CMD);  //DCDC ON
	OLED_Write_Data(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_Write_Data(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_Write_Data(0X10,OLED_CMD);  //DCDC OFF
	OLED_Write_Data(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_Write_Data (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_Write_Data (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_Write_Data (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_Write_Data(0,OLED_DATA); 
	} //更新显示
}


//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(SIZE ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_Write_Data(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_Write_Data(F8X16[c*16+i+8],OLED_DATA);
			}
			else {	
				OLED_Set_Pos(x,y+1);
				for(i=0;i<6;i++)
				OLED_Write_Data(F6x8[c][i],OLED_DATA);
				
			}
}
//m^n函数
uint32_t oled_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ');
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0'); 
	}
} 
//显示一个字符号串
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j]);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//显示汉字
void OLED_ShowChinese(uint8_t x,uint8_t y,uint8_t no)
{      			    
	uint8_t t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_Write_Data(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_Write_Data(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) 
	{
		y=y1/8;  
	}
  else 
	{
		y=y1/8+1;
	}
		
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_Write_Data(BMP[j++],OLED_DATA);	    	
	    }
	}
} 


//初始化SSD1306					    
void OLED_Init(void)
{			  
	OLED_Write_Data(0,OLED_SCLK_High);
	OLED_Write_Data(0,OLED_SDIN_High);
	OLED_Write_Data(0,OLED_RES_High);
	OLED_Write_Data(0,OLED_DC_High);
	OLED_Write_Data(0,OLED_CS_High);

	usleep(20*1000);
	OLED_Write_Data(0,OLED_RES_Low);
	usleep(20*1000);
	OLED_Write_Data(0,OLED_RES_High);
	usleep(20*1000);


	OLED_Write_Data(0xAE,OLED_CMD);//--turn off oled panel
	OLED_Write_Data(0x00,OLED_CMD);//---set low column address
	OLED_Write_Data(0x10,OLED_CMD);//---set high column address
	OLED_Write_Data(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_Write_Data(0x81,OLED_CMD);//--set contrast control register
	OLED_Write_Data(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	OLED_Write_Data(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_Write_Data(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_Write_Data(0xA6,OLED_CMD);//--set normal display
	OLED_Write_Data(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_Write_Data(0x3f,OLED_CMD);//--1/64 duty
	OLED_Write_Data(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_Write_Data(0x00,OLED_CMD);//-not offset
	OLED_Write_Data(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_Write_Data(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_Write_Data(0xD9,OLED_CMD);//--set pre-charge period
	OLED_Write_Data(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_Write_Data(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_Write_Data(0x12,OLED_CMD);
	OLED_Write_Data(0xDB,OLED_CMD);//--set vcomh
	OLED_Write_Data(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_Write_Data(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_Write_Data(0x02,OLED_CMD);//
	OLED_Write_Data(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_Write_Data(0x14,OLED_CMD);//--set(0x10) disable
	OLED_Write_Data(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_Write_Data(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_Write_Data(0xAF,OLED_CMD);//--turn on oled panel
	
	OLED_Write_Data(0xAF,OLED_CMD); /*display ON*/ 

	OLED_Clear();
	OLED_Set_Pos(0,0); 	
}  


// @显示汉字字符串函数
// @x：横坐标  
// @y：纵坐标
// @number：显示汉字字符串的首个汉字的编号
// @count：显示汉字的个数 
void OLED_ShowChineseString(int x, int y, int number, int count)
{
	int i, chinesex, chinesey, chinesea;
	for(i = 0; i < count; i ++)			// 显示汉字的个数
	{
		chinesex = x + i * 16;			// X轴从8递增	
		chinesey = y;						// Y轴不变
		chinesea = number + i;		 // 从0号汉字开始(汉字定义在oledfont.h中)
		OLED_ShowChinese(chinesex, chinesey, chinesea);
	}
}

