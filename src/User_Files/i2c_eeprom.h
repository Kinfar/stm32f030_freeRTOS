/*
 * @file	i2c_eeprom.h
 * @brief	it includes all information of this project.
 * 			head files,defined,typedef enum,struct,external functions,etc.
 * @author	dylan.wu
 * @create	2014/9/10
 * @version 1.0.0.0
 * @modify	2014/9/10
 */

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __I2C_EEPROM_H
#define __I2C_EEPROM_H

#ifdef __cplusplus
 extern "C" {
#endif 
/* Includes ------------------------------------------------------------------*/
#include "HardwareSTM32.h"
   
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
	uint8_t sEE_Init(void);
	uint8_t sEE_ReadBytes(uint8_t* pDataOut, uint8_t byteAddr, uint8_t bytesToRead);
	uint8_t sEE_WriteBytes(uint8_t* pDataIn, uint8_t byteAddr, uint8_t bytesToWrite);

#ifdef __cplusplus
}
#endif

#endif /* __I2C_EEPROM_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


