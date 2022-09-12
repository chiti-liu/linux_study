#ifndef  __OLED_API_H
#define __OLED_API_H	

#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"


//OLED模式设置
//0:4线串行模式
//1:并行8080模式
#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    


#define	uint8_t    unsigned char
#define	uint16_t  unsigned int
#define	uint32_t  unsigned long


#define OLED_CMD  0	// 写命令
#define OLED_DATA 1	// 写数据

#define OLED_SCLK_Low   10    // OLED_SCLK_Low
#define OLED_SCLK_High  11    // OLED_SCLK_High
#define OLED_SDIN_Low    12    // OLED_SDIN_Low
#define OLED_SDIN_High  13    // OLED_SDIN_High
#define OLED_RES_Low     14    // OLED_RES_Low
#define OLED_RES_High    15    // OLED_RES_High
#define OLED_DC_Low       16    // OLED_DC_Low
#define OLED_DC_High      17    // OLED_DC_High
#define OLED_CS_Low        18    // OLED_CS_Low
#define OLED_CS_High       19    // OLED_CS_High




//OLED控制用函数 
void OLED_Write_Data(uint8_t data, uint8_t cmd);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void OLED_ShowString(uint8_t x,uint8_t y, uint8_t *p);	 
void OLED_ShowChinese(uint8_t x,uint8_t y,uint8_t no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void OLED_ShowChineseString(int x, int y, int number, int count);



#endif  
	 



