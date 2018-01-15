/*
 * @file	rtc_driver.h
 * @brief	it includes all information of this project.
 * 			head files,defined,typedef enum,struct,external functions,etc.
 * @author	dylan.wu
 * @create	2014/9/10
 * @version 1.0.0.0
 * @modify	2014/9/10
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_DRIVER_H
#define __RTC_DRIVER_H

#ifdef __cplusplus
 extern "C" {
#endif
   
/* Includes ------------------------------------------------------------------*/
#include "HardwareSTM32.h"

/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
	uint8_t BSP_RTC_Init(uint16_t bkpKeyword);
	uint8_t BSP_RTC_SetDateTime(DateTime_TypeDef* dateTime);
	void BSP_RTC_GetDateTime(DateTime_TypeDef* dateTime);

  
#ifdef __cplusplus
}
#endif

#endif /* __RTC_DRIVER_H */
