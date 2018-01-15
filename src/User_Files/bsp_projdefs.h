/*
    Dylan's Embedded Source Files V1.0.0 - Copyright (C) 2014 Dylan Independent Design House.
    All rights reserved

    VISIT http://www.reserved.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    ***************************************************************************
     *                                                                       *
     *    If this document is helpful to you, please support me.    		 *
     *                                                                       *
     *    Thank you!                                                         *
     *                                                                       *
    ***************************************************************************

    This file is part of the Dylan's Embedded Source Files distribution.

    1 tab == 4 spaces!
*/



#ifndef BSP_PROJDEFS_H
#define BSP_PROJDEFS_H


/* Exported constants --------------------------------------------------------*/
/* Const to source of high speed clock:internal or external. */
#define	USE_EXTERNAL_HSE						0
#define	USE_INTERNAL_HSI						1

/* Defines const. */
#define COM_BUFF_DEPTH							( (uint16_t) (2*100) )
#define sWHILE_LONG_TIMEOUT              		( (uint16_t) 200)

/* Const to COM_TypeDef. */
#ifndef USE_COM_LED
	#define USE_COM_LED							1
#endif

#ifndef USE_CAN_LED
	#define USE_CAN_LED							1
#endif

#if ((USE_COM_LED + USE_CAN_LED) > 0)
	#define RX_LED_BIT							0
	#define TX_LED_BIT							1
#endif
/* Exported macro ------------------------------------------------------------*/
/* Define macros to split gpio type. */
#define SPLIT_PORT(xPort, uPin)					(xPort)
#define SPLIT_PIN(xPort, uPin)					(uPin)
#define SPLIT_DEFINE_PORT(xGPIO_Define)			SPLIT_PORT(xGPIO_Define)
#define SPLIT_DEFINE_PIN(xGPIO_Define)			SPLIT_PIN(xGPIO_Define)

/* Exported types ------------------------------------------------------------*/
/* A struct use to create a gpio type collection.
 * Defines the prototype to which functions must conform.
 * Defined in this file to ensure the type is known before stm32f0xx.h is included.
 */
typedef struct GPIO_Collection_TypeDef
{
	GPIO_TypeDef*	pGPIO_Port;
	uint16_t		uGPIO_Pin;
} PortCollection_TypeDef;

typedef struct RTC_DATETIME_TypeDef
{
	/* word 0 */
	uint32_t Year:12;		/* max value is 4095,and year is from 1900 to 2050 */
	uint32_t Month:4;		/* max value is 15,and month is from 1 to 12 */
	uint32_t Day:5;			/* max value is 31,and day is from 1 to 31 */
	uint32_t RTC_Year:8;
	uint32_t Reserved_1:3;	/* reserved bits */

	/* word 1 */
	uint32_t DayOfWeek:4;	/* max value is 15,and week is from 1 to 7 */
	uint32_t Hour:5;			/* max value is 31,and hour is from 0 to 23 */
	uint32_t Minute:6;		/* max value is 63,and minute is from 0 to 59 */
	uint32_t Second:6;		/* max value is 63,and second is from 0 to 59 */
	uint32_t Millisecond:10;	/* max value is 1023,and second is from 0 to 999 */
	uint32_t Reserved_2:1;	/* reserved bits */
} DateTime_TypeDef;

typedef struct USART_INTERFACE_STATUS_AND_DATA_BUFFER_TypeDef
{
	/* word 0 */	
	uint32_t ReadByte:8;
	uint32_t BytesToRead:8;
	uint32_t BytesToWrite:8;
	uint32_t PortNo:8;

	/* word 1 */
	uint32_t ReadTimer:10;
	uint32_t ReadTimeout:10;
	uint32_t ReadBufferSize:10;
	uint32_t IsReadEnable:1;
	uint32_t IsReadTimeout:1;

#if (USE_COM_LED == 1)
	/* word 2 */
	uint32_t ReadLedTimer:16;
	uint32_t WriteLedTimer:16;
#endif

	/* word N */
	uint8_t  ReadBuffer[COM_BUFF_DEPTH];
	uint8_t  WriteBuffer[COM_BUFF_DEPTH];
} COM_TypeDef;

/**
  * <b>全局变量的数据结构</b>
  * @version		5.2014.0125
  * <p>5.2014.0125：数据结构处理为DWORD边界的倍数，即u32*N。
  * @author 		Dylan
  * @see                Javadoc注释/Eclipse编辑
  */
typedef struct Global_Var_TypeDef
{
	COM_TypeDef com1,com2;

	uint8_t utc_Time[20];

	uint8_t canIsRead;

#if (USE_CAN_LED == 1)
	/* word 2 */
	uint32_t CanPortNo:4;
	uint32_t CanReadLedTimer:14;
	uint32_t CanWriteLedTimer:14;
#endif
	//uint16_t TimerCounter;        //定时器计数

	//uint16_t waitTimeout;//u16 reserved1;
}xGlobal_TypeDef;  //全局变量 整合到一起 方便管理


/*

	***************************************************************************
	 *                                                                       *
	 *    Above amendments before September 21, 2014.   		             *
	 *                                                                       *
	***************************************************************************

 */

/* Converts a time in milliseconds to a time in ticks. */


/* Error definitions. */
/*
#define errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY	( -1 )
#define errQUEUE_BLOCKED						( -4 )
#define errQUEUE_YIELD							( -5 )
*/

#endif /* BSP_PROJDEFS_H */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/

/*
 * Keil 编译后 Flash 和 RAM 占用情况：
 * Program Size: Code=10266 RO-data=354 RW-data=68 ZI-data=3836
 *
 * Code -- 代码
 * RO-data -- 常量
 * RW-data -- 可读写变量，已初始化
 * ZI-data -- 可读写变量，未初始化
 *
 * 以上flash和ram占用情况：
 * flash = Code + RO-data + RW-data = 10266 + 354 + 68 = 10688 bytes
 * ram = RW-data + ZI-data = 68 + 3836 = 3899 bytes
*/




