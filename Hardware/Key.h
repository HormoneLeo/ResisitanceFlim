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
/*ͷ�ļ�*/
#include "stm32f10x.h" 
#include "string.h"
#include "Delay.h"
#include "OLED.h"    
/*�궨��*/
#define KEY_NUM 4//��������
/*�ṹ�����Ͷ���*/
typedef struct 
{
	uint32_t Key_RCC_GPIO;//�����˿ڱ��ʱ��

	GPIO_TypeDef* Key_GPIO_Port;//�����˿ڱ��

	uint16_t Key_GPIO_Pin;//�����˿����ź�

	BitAction Key_State;//����״̬

}Key_InitTypeDef;
/*����*/
void Key_Init(Key_InitTypeDef Key_InitArray[]);
uint16_t Key_Scan(Key_InitTypeDef Key_InitArray[]);
#endif
