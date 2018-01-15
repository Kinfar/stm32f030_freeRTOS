/*
 * @file	HardwareSTM32.h
 * @brief	it includes all information of this project.
 * 			head files,defined,typedef enum,struct,external functions,etc.
 * @author	dylan.wu
 * @create	2014/4/2
 * @version 1.0.0.0
 * @modify	2014/4/2
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HARDWARESTM32_H
#define __HARDWARESTM32_H

#ifdef __cplusplus
 extern "C" {
#endif
   
/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/*******************************************************************************
  电路板设定
  IO悬空==>>>>>>  //PA1用作timer2——ch 输出
  IO接地==>>>>>>  //不用的引脚、且模拟输入Analog Input mode
*******************************************************************************/
/** @brief define all pins which is used in demo board.
  * @{
  */
#if  defined (STM32F042)
  /* GPIOB allpins */
	#define GPIO_SW01							GPIOB, GPIO_Pin_0
	#define GPIO_SW02							GPIOB, GPIO_Pin_9
	#define GPIO_SW03							GPIOB, GPIO_Pin_8
	#define GPIO_SW04							GPIOB, GPIO_Pin_7
	#define GPIO_SW05							GPIOB, GPIO_Pin_6
	#define GPIO_SW06							GPIOB, GPIO_Pin_5
	#define GPIO_SW07							GPIOB, GPIO_Pin_4
	#define GPIO_SW08							GPIOB, GPIO_Pin_3
	#define GPIO_SW09							GPIOB, GPIO_Pin_1
	#define GPIO_SW10							GPIOB, GPIO_Pin_2
	#define GPIO_SW11							GPIOB, GPIO_Pin_10
	#define GPIO_SW12							GPIOB, GPIO_Pin_11
	#define GPIO_SW13							GPIOB, GPIO_Pin_12
	#define GPIO_SW14							GPIOB, GPIO_Pin_13
	#define GPIO_SW15							GPIOB, GPIO_Pin_14
	#define GPIO_SW16							GPIOB, GPIO_Pin_15
	
	/* PA0 reuse to wakeup or timer irq. */
	#define ST_TIM2_CH1           				GPIOA, GPIO_Pin_0
	#define WakeUp								GPIOA, GPIO_Pin_0
	
	#define RevIN								GPIOA, GPIO_Pin_1
	
	/* PA2,3 to pl2303hx or Com2. */
	#define ST_COM2_Tx							GPIOA, GPIO_Pin_2
	#define ST_COM2_Rx							GPIOA, GPIO_Pin_3
	#define PL2303HX_TX							ST_COM2_Tx
	#define PL2303HX_RX							ST_COM2_Rx
	
	#define ST_SPI1_CS							GPIOA, GPIO_Pin_4
	#define ST_SPI1_SCK							GPIOA, GPIO_Pin_5
	#define ST_SPI1_MISO						GPIOA, GPIO_Pin_6
	#define ST_SPI1_MOSI						GPIOA, GPIO_Pin_7
	
	/* PA9,10 to rs485 or Com1. */
	#define ST_COM1_Tx							GPIOA, GPIO_Pin_9
	#define ST_COM1_Rx							GPIOA, GPIO_Pin_10
	/* PA4 reuse to rs485 de pin*/
	#define ST_COM1_DE							ST_SPI1_CS
	
	#define RS485_TX							ST_COM1_Tx
	#define RS485_RX							ST_COM1_Rx	
	#define RS485_DE							ST_COM1_DE
	
	#define ST_MCO								GPIOA, GPIO_Pin_8
	
	/* PA11,12 remap to can or iic */
	#define ST_CAN1_Rx							GPIOA, GPIO_Pin_11
	#define ST_CAN1_Tx							GPIOA, GPIO_Pin_12
	#define CAN1_STBY							GPIOA, GPIO_Pin_15
	
	#define ST_I2C1_SCL							ST_CAN1_Rx
	#define ST_I2C1_SDA							ST_CAN1_Tx
	
	#define SetAF_COM1_Tx         {GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);}
	#define SetAF_COM1_Rx         {GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);}
	
	#define SetAF_COM2_Tx         {GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);}
	#define SetAF_COM2_Rx         {GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);}
	
	#define SetAF_TIM2_CH1        {GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_2);}
	
	#define SetAF_CAN1_Tx         {GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_4);}	
	#define SetAF_CAN1_Rx         {GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_4);}
	
	#define SetAF_I2C1_SCL        {GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_5);}
	#define SetAF_I2C1_SDA        {GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_5);}
	
	#define SetAF_SPI1_SCK        {GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_0);}
	#define SetAF_SPI1_MISO       {GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_0);}
	#define SetAF_SPI1_MOSI       {GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_0);}
#else
	#warning "Make sure needn't define STM32F042 etc. ?"
#endif
/**
  * @}
  */

#define DELAY_US(time)		Delay_us(time, SystemClockMHz)
#define DELAY_MS(time)		Delay_us(time*1000, SystemClockMHz)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

	void connectPinsToCorrectPosition(void);
	void connectUsart1ToExt(uint32_t baudrate, u8 parity, u8 length, u8 stopbits);
	void Delay_us(uint32_t, u8);
	bool Flash_wWord(uint32_t, u16);
	bool Flash_wWords(uint32_t, u16*, u16);
	void createMasterClock(uint8_t oscSelect,u8 clkMHz);
	void StartIWDG(u16 resetTimeout);
	void FeedIWDG(void);
	void connectTimBase1US(TIM_TypeDef * TIMx, u8 SystemCLK, u16 LoadingValue);

#if  defined (STM32F042)
	void connectUsart2ToExt(uint32_t baudrate, u8 parity, u8 length, u8 stopbits);
	void connectSPI1ToExt(u16 BaudRatePCLKDiv, u16 CPOL, u16 CPHA_Edge);

#elif defined (STM32F100) || defined (STM32F101) || defined (STM32F103)
	void NVIC_GroupSet(uint32_t NVIC_PriorityGroup, s16 NVIC_IRQn, u16 NVIC_Priority);
	
#else
	#warning "Make sure needn't define STM32F051 etc. ?"
#endif
  
#ifdef __cplusplus
}
#endif

#endif /* __HARDWARESTM32_H */
