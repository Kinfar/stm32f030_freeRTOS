/*
 * @file	Software.c
 * @brief
 * @author	dylan.wu
 * @create	2014/4/2
 * @modify	2014/4/2
 * @version 1.0.0.0
 */

/* Includes ------------------------------------------------------------------*/
#include  "Software.h"
#include  "HardwareSTM32.h"
#include  "usart_driver.h"
#include  "can_driver.h"
#include  "i2c_eeprom.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern xGlobal_TypeDef glvar;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

  
/**
  * <b>系统初始化函数。</b>
  * <p>初始化外设、初始化全局变量、配置中断优先级
  * @version		v1.0(2013-12-5)
  * <p>重写函数的功能说明！
  * @author 		Dylan
  * @see                Javadoc注释/Eclipse编辑
  */
void BSP_Hardware_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	createMasterClock(USE_EXTERNAL_HSE, SystemClockMHz);
  
	connectPinsToCorrectPosition();
  
	BSP_USART_Init(USART1, 9600, 0, 8, 1);
	BSP_USART_Init(USART2, 9600, 0, 8, 1);

	COM_DeInit(&glvar.com1);
	glvar.com1.PortNo = 0;
	glvar.com1.ReadTimeout = 10;

	COM_DeInit(&glvar.com2);
	glvar.com2.PortNo = 0;
	glvar.com2.ReadTimeout = 10;
  
	connectTimBase1US(TIM2, SystemClockMHz, 1000);
	KEY_WakeUpConfig();
	
	#ifndef __TestCAN
		BSP_CAN_Init(250);

		glvar.CanPortNo = 6;
	#else
		sEE_Init();
	#endif

	//Interrupts
	NVIC_InitStructure.NVIC_IRQChannel = CEC_CAN_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


/*******************************************************************************
* Function Name  : KEY_WakeUpConfig
* Description    : 
1.打开GPIOA与GPIO复用的时钟
2.将PA0配置成EXTI_0
3.配置NVIC中断通道
*******************************************************************************/
void KEY_WakeUpConfig(void)//PA0按钮
{
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStructure);
}


