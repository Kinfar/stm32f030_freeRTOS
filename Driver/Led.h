#ifndef _LED_H_
#define _LED_H_
#include "stm32f0xx.h"

#define u8 uint8_t
#define u16 uint16_t

#define LED1_OFF  	GPIO_SetBits(GPIOB, GPIO_Pin_11)
#define LED1_ON 	GPIO_ResetBits(GPIOB, GPIO_Pin_11)
#define LED2_OFF  	GPIO_SetBits(GPIOA, GPIO_Pin_15)
#define LED2_ON 	GPIO_ResetBits(GPIOA, GPIO_Pin_15)

#define LEDALL_OFF  GPIO_SetBits(GPIOB, GPIO_Pin_11);GPIO_SetBits(GPIOA, GPIO_Pin_15)
#define LEDALL_ON 	GPIO_ResetBits(GPIOB, GPIO_Pin_11);GPIO_ResetBits(GPIOA, GPIO_Pin_15)

//设置飞控状态的四个按键
#define KEY1_ON 	!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)
#define KEY1_OFF 	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)
#define KEY2_ON 	!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)
#define KEY2_OFF 	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)

void LED_INIT(void);
void LEDALL_FLASH(u8 times, u16 time);
void Key_INIT(void);

#endif


