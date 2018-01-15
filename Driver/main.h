/*
                 ////////////

           //////////////////////////                   /////////////////////////////
					 ////                /////                               //////
					 ////                /////                               //////
				 /////                /////               /////////////////////////////////////////
				//////////////////////////                             //////  /////
			 /////                                                  //////     /////
			////    ///////////////////                            //////         /////
		////      ////          /////                           /////            //////
	 ////       ////          /////                          /////              ///////
	////        ////          /////                         /////                ////////
////          ///////////////////                        /////                   ////////
//启天科技出品
//研发团队：启天四轴团队
//联系方式：QQ群：471023785
*/
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "sysconfig.h"
#include "delay.h"
#include "string.h"
#include "stdio.h"
#include "spi.h"
#include "nrf24l01.h"
#include "led.h"
#include "math.h"
#include "drv_adc.h"



/* Exported typedef ----------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
