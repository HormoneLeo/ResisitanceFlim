#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"
#include "LED.h"
#include "Key.h"


Key_InitTypeDef Keys_t[KEY_NUM] = {
	{// 右
		.Key_RCC_GPIO = RCC_APB2Periph_GPIOA,
		.Key_GPIO_Port = GPIOA,
		.Key_GPIO_Pin = GPIO_Pin_0},
	{// 左
		.Key_RCC_GPIO = RCC_APB2Periph_GPIOA,
		.Key_GPIO_Port = GPIOA,
		.Key_GPIO_Pin = GPIO_Pin_1},
	{// 上	
		.Key_RCC_GPIO = RCC_APB2Periph_GPIOB,
		.Key_GPIO_Port = GPIOB,
		.Key_GPIO_Pin = GPIO_Pin_1},
	{// 下
		.Key_RCC_GPIO = RCC_APB2Periph_GPIOB,
		.Key_GPIO_Port = GPIOB,
		.Key_GPIO_Pin = GPIO_Pin_0}

};

struct
{
	/* data */
	uint8_t Site[40][3];
	uint8_t Site_RegNum;
	// uint8_t pSite_RegNum;
} Data_t;

int main(void)
{
	OLED_Init();
	AD_Init();
	LED_Init();
	Key_Init(Keys_t);

	OLED_ShowString(37, 56, "(  ,  )", OLED_6X8);
	OLED_DrawRectangle(13, 0, 112, 56, OLED_UNFILLED);

	while (1)
	{

		Delay_ms(20);

		if ((Key_Scan(Keys_t) & (0x0001 << 0)) != 0)
		{

			if (Data_t.Site_RegNum < 40)
			{

				Data_t.Site[Data_t.Site_RegNum][0] = (int)((1 - (float)AD_Value[1] / (float)AD_Value[2]) * 30);
				Data_t.Site[Data_t.Site_RegNum][1] = (int)((1 - (float)AD_Value[0] / (float)AD_Value[2]) * 20);
				Data_t.Site[Data_t.Site_RegNum][2] = 0;
				Data_t.Site_RegNum++;
			}
		}
		else if ((Key_Scan(Keys_t) & (0x0001 << 1)) != 0)
		{
			if (Data_t.Site_RegNum < 40)
			{

				Data_t.Site[Data_t.Site_RegNum][0] = (int)((1 - (float)AD_Value[1] / (float)AD_Value[2]) * 30);
				Data_t.Site[Data_t.Site_RegNum][1] = (int)((1 - (float)AD_Value[0] / (float)AD_Value[2]) * 20);
				if (Data_t.Site_RegNum == 0)
					Data_t.Site[Data_t.Site_RegNum][2] = 0;
				else
					Data_t.Site[Data_t.Site_RegNum][2] = 1;

				Data_t.Site_RegNum++;
			}
		}
		else if ((Key_Scan(Keys_t) & (0x0001 << 2)) != 0)
		{
			if (Data_t.Site_RegNum > 0)
			{
				Data_t.Site_RegNum--;
				Data_t.Site[Data_t.Site_RegNum][0] = 0;
				Data_t.Site[Data_t.Site_RegNum][1] = 0;
				Data_t.Site[Data_t.Site_RegNum][2] = 0;
			}
		}
		else if ((Key_Scan(Keys_t) & (0x0001 << 3)) != 0)
		{
			for (uint8_t i = 0; i < Data_t.Site_RegNum; i++)
			{
				Data_t.Site[i][0] = 0;
				Data_t.Site[i][1] = 0;
				Data_t.Site[i][2] = 0;
			}
			Data_t.Site_RegNum = 0;
		}
		Delay_ms(20);

		OLED_ClearArea(14, 1, 110, 54);
		OLED_ShowNum(37 + 8, 56, Data_t.Site[Data_t.Site_RegNum - 1][0], 2, OLED_6X8);
		OLED_ShowNum(37 + 8 * 3, 56, Data_t.Site[Data_t.Site_RegNum - 1][1], 2, OLED_6X8);
		for (uint16_t i = 0; i < Data_t.Site_RegNum; i++)
		{
			uint8_t X0, Y0;
			X0 = 13 + (int)((float)Data_t.Site[i][0] / 30 * 112);
			;
			Y0 = 55 - (int)((float)Data_t.Site[i][1] / 20 * 56);

			if (Data_t.Site[i][2] != 0)
			{
				uint8_t X1, Y1;
				X1 = 13 + (int)((float)Data_t.Site[i - 1][0] / 30 * 112);
				;
				Y1 = 55 - (int)((float)Data_t.Site[i - 1][1] / 20 * 56);
				OLED_DrawLine(X0, Y0, X1, Y1);
				OLED_DrawLine(X0 + 1, Y0 + 1, X1 + 1, Y1 + 1);
			}

			else
			{
				OLED_DrawPoint(X0, Y0);
			}

			/* code */
		}

		OLED_Update();

		Delay_ms(100);
	}
}
