/*
 * @file	Software.h
 * @brief	it includes all information of this project.
 * 			head files,defined,typedef enum,struct,external functions,etc.
 * @author	dylan.wu
 * @create	2014/4/2
 * @modify	2014/4/2
 * @version 1.0.0.0
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SOFTWARE_H
#define __SOFTWARE_H

#ifdef __cplusplus
	extern "C" {
#endif
   
/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
	void BSP_Hardware_Init(void);
	void KEY_WakeUpConfig(void);

#ifdef __cplusplus
	}
#endif

#endif /* __SOFTWARE_H */

