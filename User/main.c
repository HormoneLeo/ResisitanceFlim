#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"
#include "LED.h"
#include "Key.h"

Key_InitTypeDef Keys_t[KEY_NUM] = {
	{.Key_RCC_GPIO = RCC_APB2Periph_GPIOA,
	 .Key_GPIO_Port = GPIOA,
	 .Key_GPIO_Pin = GPIO_Pin_0},
	{.Key_RCC_GPIO = RCC_APB2Periph_GPIOA,
	 .Key_GPIO_Port = GPIOA,
	 .Key_GPIO_Pin = GPIO_Pin_1},
	{.Key_RCC_GPIO = RCC_APB2Periph_GPIOB,
	 .Key_GPIO_Port = GPIOB,
	 .Key_GPIO_Pin = GPIO_Pin_1},
	{.Key_RCC_GPIO = RCC_APB2Periph_GPIOB,
	 .Key_GPIO_Port = GPIOB,
	 .Key_GPIO_Pin = GPIO_Pin_0}

};

struct
{
	/* data */
	uint8_t Site[40][2];
	uint8_t Site_RegNum;
	uint8_t pSite_RegNum;
} Data_t;

int main(void)
{
	OLED_Init();
	AD_Init();
	LED_Init();
	Key_Init(Keys_t);
	Data_t.Site_RegNum = 1;
	OLED_ShowString(0, 0, "Y:", OLED_6X8);
	OLED_ShowString(0, 10, "X:", OLED_6X8);
	OLED_ShowString(0, 20, "V:", OLED_6X8);
	OLED_ShowString(70, 0, "P:", OLED_6X8);
	OLED_ShowString(70, 10, "M:", OLED_6X8);
	OLED_ShowString(70, 42, "(  ,  )", OLED_6X8);
	OLED_DrawLine(0, 32, 0, 62);
	OLED_DrawLine(0, 62, 62, 62);
	OLED_DrawLine(62, 62, 62, 32);
	OLED_DrawLine(62, 32, 0, 32);

	while (1)
	{

		OLED_ShowFloatNum(16, 0, (double)AD_Value[0] / 4095 * 3.3, 1, 5, OLED_6X8);
		OLED_ShowFloatNum(16, 10, (double)AD_Value[1] / 4095 * 3.3, 1, 5, OLED_6X8);
		OLED_ShowFloatNum(16, 20, (double)AD_Value[2] / 4095 * 3.3, 1, 5, OLED_6X8);

		Delay_ms(30);
		if ((Key_Scan(Keys_t) & (0x0001 << 2)) != 0)
		{
			if (Data_t.pSite_RegNum-- <= 0)
				Data_t.pSite_RegNum = Data_t.Site_RegNum - 1;
		}
		else if ((Key_Scan(Keys_t) & (0x0001 << 3)) != 0)
		{
			if (Data_t.pSite_RegNum++ >= Data_t.Site_RegNum - 1)
				Data_t.pSite_RegNum = 0;
		}
		else if ((Key_Scan(Keys_t) & (0x0001 << 0)) != 0)
		{

			if (Data_t.Site_RegNum < 40)
			{

				Data_t.Site[Data_t.Site_RegNum][0] = (int)((1 - (float)AD_Value[1] / (float)AD_Value[2]) * 30);
				Data_t.Site[Data_t.Site_RegNum][1] = (int)((1 - (float)AD_Value[0] / (float)AD_Value[2]) * 20);
				Data_t.Site_RegNum++;
			}

			Data_t.pSite_RegNum = Data_t.Site_RegNum - 1;
		}
		else if ((Key_Scan(Keys_t) & (0x0001 << 1)) != 0)
		{
			if (Data_t.Site_RegNum > 1)
			{
				Data_t.Site[Data_t.Site_RegNum][0] = 0;
				Data_t.Site[Data_t.Site_RegNum][1] = 0;
				Data_t.Site_RegNum--;
			}

			Data_t.pSite_RegNum = Data_t.Site_RegNum - 1;
		}
		Delay_ms(30);

		OLED_ShowNum(86, 0, Data_t.pSite_RegNum + 1, 2, OLED_6X8);
		OLED_ShowNum(86, 10, Data_t.Site_RegNum, 2, OLED_6X8);
		OLED_ShowNum(78, 42, Data_t.Site[Data_t.pSite_RegNum][0], 2, OLED_6X8);
		OLED_ShowNum(94, 42, Data_t.Site[Data_t.pSite_RegNum][1], 2, OLED_6X8);
		OLED_ClearArea(1, 33, 60, 28);
		if (Data_t.pSite_RegNum > 0)
		{
			for (uint16_t i = 0; i < Data_t.pSite_RegNum; i++)
			{
				uint8_t X0, Y0, X1, Y1;
				X0 = 1 + 2 * Data_t.Site[i][0];
				Y0 = 61 - (int)((float)Data_t.Site[i][1] / 20 * 28);
				X1 = 1 + 2 * Data_t.Site[i + 1][0];
				Y1 = 61 - (int)((float)Data_t.Site[i + 1][1] / 20 * 28);
				/* code */
				OLED_DrawLine(X0, Y0, X1, Y1);
			}
		}

		OLED_Update();

		Delay_ms(100);
	}
}
