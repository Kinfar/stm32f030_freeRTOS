/*
 * @file	usart_driver.h
 * @brief	it includes all information of this project.
 * 			head files,defined,typedef enum,struct,external functions,etc.
 * @author	dylan.wu
 * @create	2014/9/10
 * @version 1.0.0.0
 * @modify	2014/9/10
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_DRIVER_H
#define __USART_DRIVER_H

#ifdef __cplusplus
 extern "C" {
#endif
   
/* Includes ------------------------------------------------------------------*/
#include  "HardwareSTM32.h"
#include  "led_driver.h"
#include  "FreeRTOS.h"
#include  "timers.h"

/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
	uint8_t BSP_USART_Init(USART_TypeDef* USARTx, u32 baudRate, u8 parity, u8 length, u8 stopBits);
	void COM_ReadEnable(COM_TypeDef* COMx);
	void COM_DeInit(COM_TypeDef* COMx);
	void COM_Init(COM_TypeDef* COMx, uint8_t rxCacheDepth, uint16_t rxTimeout);
	void USART_IRQFunction(USART_TypeDef* USARTx, COM_TypeDef* COMx);
	void USART_TimeoutFunction(COM_TypeDef* COMx);
	void COM_SendData(USART_TypeDef* USARTx, COM_TypeDef* COMx, uint8_t start, uint8_t length);
	void COM_SendBytes(USART_TypeDef* USARTx, uint8_t* pBuff, uint8_t length);
	uint8_t COM_SendEachOther(COM_TypeDef* COMx_Src, COM_TypeDef* COMx_Dest, USART_TypeDef* byUSARTx);
  
#ifdef __cplusplus
}
#endif

#endif /* __USART_DRIVER_H */
