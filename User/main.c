#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"
#include "LED.h"
#include "Key.h"

//Rec_LeftUpY + Rec_Height < Axis_ShowY
#define Rec_LeftUpX 13
#define Rec_LeftUpY 0
#define Rec_Width 112
#define Rec_Height 56	
#define Axis_ShowX 37
#define Axis_ShowY 56
#define AxisWidth_ShowMax 20
#define AxisHeight_ShowMax 30
#define PointNumMax 40

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
	uint8_t Site[PointNumMax][3];
	uint8_t Site_RegNum;
} Data_t;

int main(void)
{
	OLED_Init();
	AD_Init();
	LED_Init();
	Key_Init(Keys_t);

	OLED_ShowString(Axis_ShowX, Axis_ShowY, "(  ,  )", OLED_6X8);
	OLED_DrawRectangle(Rec_LeftUpX, Rec_LeftUpY, Rec_Width, Rec_Height, OLED_UNFILLED);

	while (1)
	{
		Delay_ms(20);
		if ((Key_Scan(Keys_t) & (0x0001 << 0)) != 0)
		{
			if (Data_t.Site_RegNum < PointNumMax)
			{

				Data_t.Site[Data_t.Site_RegNum][0] = \
				(int)((1.0f - (float)AD_Value[1] / (float)AD_Value[2]) * AxisHeight_ShowMax);

				Data_t.Site[Data_t.Site_RegNum][1] = \
				(int)((1.0f - (float)AD_Value[0] / (float)AD_Value[2]) * AxisWidth_ShowMax);

				Data_t.Site[Data_t.Site_RegNum][2] = 0;
				Data_t.Site_RegNum++;
			}
		}
		else if ((Key_Scan(Keys_t) & (0x0001 << 1)) != 0)
		{
			if (Data_t.Site_RegNum < PointNumMax)
			{

				Data_t.Site[Data_t.Site_RegNum][0] = \
				(int)((1.0f - (float)AD_Value[1] / (float)AD_Value[2]) * AxisHeight_ShowMax);

				Data_t.Site[Data_t.Site_RegNum][1] = \
				(int)((1.0f - (float)AD_Value[0] / (float)AD_Value[2]) * AxisWidth_ShowMax);
				
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



		OLED_ClearArea(Rec_LeftUpX +1 , Rec_LeftUpY + 1, Rec_Width - 2, Rec_Height - 2);
		OLED_ShowNum(Axis_ShowX + 8		, Axis_ShowY, Data_t.Site[Data_t.Site_RegNum - 1][0], 2, OLED_6X8);
		OLED_ShowNum(Axis_ShowX + 8 * 3	, Axis_ShowY, Data_t.Site[Data_t.Site_RegNum - 1][1], 2, OLED_6X8);



		for (uint16_t i = 0; i < Data_t.Site_RegNum; i++)
		{
			uint8_t X0, Y0;
			X0 = Rec_LeftUpX + 	(int)((float)Data_t.Site[i][0] / AxisHeight_ShowMax * Rec_Width);
			Y0 = Rec_LeftUpY + Rec_Height - (int)((float)Data_t.Site[i][1] / AxisWidth_ShowMax * Rec_Height);
			if (Data_t.Site[i][2] != 0)
			{
				uint8_t X1, Y1;
				X1 = Rec_LeftUpX + (int)((float)Data_t.Site[i - 1][0] / AxisHeight_ShowMax * Rec_Width);
				Y1 = Rec_LeftUpY + Rec_Height - (int)((float)Data_t.Site[i - 1][1] / AxisWidth_ShowMax * Rec_Height);
				OLED_DrawLine(X0, Y0, X1, Y1);
				OLED_DrawLine(X0 + 1, Y0 + 1, X1 + 1, Y1 + 1);
			}
			else
			{
				OLED_DrawPoint(X0, Y0);
			}
		}
		OLED_Update();
		Delay_ms(100);
	}
}
