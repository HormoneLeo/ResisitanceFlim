/**
 * @file key.c
 * @author Zuhao Lei
 * @brief 
 * @version 0.1
 * @date 2023-11-30
 * 
 * 
 */
#include "key.h"




/**
 * @brief 按键遍历初始化
 * 
 * @param Key_InitArray 按键结构体数组
 */
void Key_Init(Key_InitTypeDef Key_InitArray[])
{
	for (uint16_t i = 0; i < KEY_NUM; i++)
	{
		/* code */
		RCC_APB2PeriphClockCmd(Key_InitArray[i].Key_RCC_GPIO, ENABLE);
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_IPU;//上拉输入模式
		GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_50MHz;

		GPIO_InitStructure.GPIO_Pin 	= 	Key_InitArray[i].Key_GPIO_Pin ;
	GPIO_Init(Key_InitArray[i].Key_GPIO_Port, &GPIO_InitStructure);
	}
	
	
}




/**
 * @brief 按键状态遍历扫描
 * 
 * @param Key_InitArray 按键结构体数组
 * @return 每一位代表对应位按钮的状态，1表示被按下，0表示未按下 
 */
uint16_t Key_Scan(Key_InitTypeDef Key_InitArray[])
{
	uint16_t Key_States = 0x0000;
	for (uint16_t i = 0; i < KEY_NUM; i++)
	{
		/* code */
		if(GPIO_ReadInputDataBit(Key_InitArray[i].Key_GPIO_Port,Key_InitArray[i].Key_GPIO_Pin ) == 0){
			Key_InitArray[i].Key_State = SET;
			Key_States |= ( 0x0001 << i);
		}
		else{
			Key_InitArray[i].Key_State = RESET;
			Key_States &= ~( 0x0001 << i);
		}	
	}
	return Key_States;
}

