
#include "oled_api.h"
#include "oled_font.h"
#include "oled_bmp.h"


/*
 * @description : main 主程序
 * @param - argc : argv 数组元素个数
 * @param - argv : 具体参数
 * @return : 0 成功;其他 失败
 */
int main(int argc, char *argv[])
{
	if(argc != 1)
	{
		printf("error\r\n");
		return 0;
	}

	OLED_Init();

	while(1)
	{
		OLED_Clear();
		OLED_DrawBMP(0, 0, 128, 8, BMP2);
		usleep(1000*1000);

		OLED_Clear();
		OLED_DrawBMP(0, 0, 128, 8, BMP3);
		usleep(1000*1000);

		OLED_Clear();
		OLED_DrawBMP(0, 0, 128, 8, BMP4);
		usleep(1000*1000);

		OLED_Clear();
		OLED_DrawBMP(0, 0, 128, 8, BMP5);
		usleep(1000*1000);

		OLED_Clear();
		OLED_DrawBMP(0, 0, 128, 8, BMP6);
		usleep(1000*1000);

		OLED_Clear();
		OLED_DrawBMP(0, 0, 128, 8, BMP7);
		usleep(1000*1000);

		OLED_Clear();
		OLED_ShowChineseString(16, 0, 0, 6);
		usleep(1000*1000);

		OLED_Clear();
		OLED_ShowChineseString(48, 3, 6, 3);
		usleep(2000*1000);

		OLED_Clear();
		OLED_ShowChineseString(48, 6, 6, 3);
		usleep(3000*1000);
		
	}

}

