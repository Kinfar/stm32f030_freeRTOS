/*
 * @file	can_driver.h
 * @brief	it includes all information of this project.
 * 			head files,defined,typedef enum,struct,external functions,etc.
 * @author	dylan.wu
 * @create	2014/9/10
 * @version 1.0.0.0
 * @modify	2014/9/10
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CAN_DRIVER_H
#define __CAN_DRIVER_H

#ifdef __cplusplus
 extern "C" {
#endif
   
/* Includes ------------------------------------------------------------------*/
#include "HardwareSTM32.h"
#include "usart_driver.h"

/* Exported types ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
	void BSP_CAN_Init(uint16_t kbpsBaudRate);
	void BSP_MCP2562_Enable(void);
	void BSP_MCP2562_Disable(void);

  
#ifdef __cplusplus
}
#endif

#endif /* __CAN_DRIVER_H */
