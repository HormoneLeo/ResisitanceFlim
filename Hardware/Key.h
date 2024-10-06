/**
 * @file key.h
 * @author Zuhao Lei
 * @brief 
 * @version 0.1
 * @date 2023-11-30
 * 
 */
#ifndef __KEY_H
#define __KEY_H
/*头文件*/
#include "stm32f10x.h" 
#include "string.h"
#include "Delay.h"
#include "OLED.h"    
/*宏定义*/
#define KEY_NUM 4//按键数量
/*结构体类型定义*/
typedef struct 
{
	uint32_t Key_RCC_GPIO;//按键端口编号时钟

	GPIO_TypeDef* Key_GPIO_Port;//按键端口编号

	uint16_t Key_GPIO_Pin;//按键端口引脚号

	BitAction Key_State;//按键状态

}Key_InitTypeDef;
/*函数*/
void Key_Init(Key_InitTypeDef Key_InitArray[]);
uint16_t Key_Scan(Key_InitTypeDef Key_InitArray[]);
#endif
