/*
 * @file	includes.h
 * @brief	it includes all information of this project.
 * 			head files,defined,typedef enum,struct,external functions,etc.
 * @author	dylan.wu
 * @create	2014/4/1
 * @modify	2014/4/1
 * @version 1.0.0.0
 */

/*
 * Keil ����� Flash �� RAM ռ�������
 * Program Size: Code=10266 RO-data=354 RW-data=68 ZI-data=3836
 *
 * Code -- ����
 * RO-data -- ����
 * RW-data -- �ɶ�д�������ѳ�ʼ��
 * ZI-data -- �ɶ�д������δ��ʼ��
 *
 * ����flash��ramռ�������
 * flash = Code + RO-data + RW-data = 10266 + 354 + 68 = 10688 bytes
 * ram = RW-data + ZI-data = 68 + 3836 = 3899 bytes
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __INCLUDES_H
#define __INCLUDES_H

#ifdef __cplusplus
 extern "C" {
#endif

/** @brief define data type's alias
  * @{
  */
#ifndef __Define_DataTypeAlias
#define __Define_DataTypeAlias
	typedef unsigned char u8;
	typedef unsigned short int u16;
	typedef unsigned int u32;

	typedef signed char  s8;
	typedef signed short int s16;
	typedef signed int s32;

	//volatile�����ã� ��Ϊָ��ؼ��֣�ȷ����������������Ż������±���ָ�ʡ��.
	typedef volatile u8 vu8;
	typedef volatile u16 vu16;
	typedef volatile u32 vu32;

	typedef volatile s8 vs8;
	typedef volatile s16 vs16;
	typedef volatile s32 vs32;
	
	#ifndef __cplusplus
	typedef enum {FALSE=0, TRUE=!FALSE}bool;
	#endif
#endif
/**
  * @}
  */

/** @brief user configure of embeded system
  * @{
  */
	#define SystemClockMHz                	((u8)48) /* unit is MHz. */

	#define Current_FlashSizeKB     				((u8)16) /* unit is KB. ��ʾ��ǰMCU�����Flash����. ÿ������MCUʱ����Ҫcheck. */
	#define Current_ProgramSizeKB						((u8)10) /* unit is KB. ��ʾ��ǰproject�ĳ�������. ÿ��ReBuild���̺���Ҫcheck. */
	#define FlashKeywordsAddress          	((u32)(0x08000000 + (Current_ProgramSizeKB<<10)))  /* ������������һҳ��ʼ��ַ. check the flash ROM had be setting?  */
	#define FlashKeywords                 	((u16)0xA1A1) /* write the word after configuration. */
	#define FlashDataAddress              	((u32)(FlashKeywordsAddress + 0x400)) /* make sure it's at the next page of code address! */

	/*
	#define	KeyPressLongMax									((u16)600)
	#define	KeyPressShortMax								((u16)400)
	#define	KeyPressNoneMax									((u16)4)*/
/**
  * @}
  */

/* Includes ------------------------------------------------------------------*/
#if defined (STM32F051) || defined (STM32F030) || defined (STM32F042)
	#include "stm32f0xx.h"
#elif defined (STM32F100) || defined (STM32F101) || defined (STM32F103)
	#include "stm32f10x.h"
#else
	#warning "Make sure needn't define STM32F051 etc. ?"
#endif

#include "bsp_projdefs.h"
#include "FreeRTOS.h"

	u8 SpiTransaction(u8 byte);
#ifdef __cplusplus
  }
#endif

#endif /* __INCLUDES_H */
