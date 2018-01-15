/*
 * @file	led_driver.h
 * @brief	it includes all information of this project.
 * 			head files,defined,typedef enum,struct,external functions,etc.
 * @author	dylan.wu
 * @create	2014/4/2
 * @version 1.0.0.0
 * @modify	2014/4/2
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LED_DRIVER_H
#define __LED_DRIVER_H

#ifdef __cplusplus
 extern "C" {
#endif
   
/* Includes ------------------------------------------------------------------*/
#include "HardwareSTM32.h"

/* Exported types ------------------------------------------------------------*/
/* Exported defines ----------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
	void BSP_LED_Input_Init(void);
	void BSP_LED_Output_Init(void);
	void LED_ON(uint8_t);
	void LED_OFF(uint8_t);
	void LED_AllOn(void);
	void LED_AllOff(void);
	void LED_Neon1(uint8_t);
	void LED_Neon2(uint8_t);
	void LEDs_DisplayHex(uint16_t);
  
#ifdef __cplusplus
}
#endif

#endif /* __LED_DRIVER_H*/
