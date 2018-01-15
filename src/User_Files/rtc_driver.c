/**
  ******************************************************************************
  * @file /rtc_driver.c
  * @author  xd.wu
  * @brief  基于STM32_RTC的实时时钟程序。
  * RTC的配置过程：
  * 1. 查询RTOFF位，直到RTOFF的值变为’1’
  * 2. 置CNF值为1，进入配置模式
  * 3. 对一个或多个RTC寄存器进行写操作
  * 4. 清除CNF标志位，退出配置模式
  * 5. 查询RTOFF，直至RTOFF位变为’1’以确认写操作已经完成。
  * 仅当CNF标志位被清除时，写操作才能进行，这个过程至少需要3个RTCCLK周期。
  * @last_modify_date	2014.9.23
  ******************************************************************************
  * 
  */

/* Includes ------------------------------------------------------------------*/
#include  "rtc_driver.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#if ( INCLUDE_vTaskDelay == 1 )
	#include "task.h"

	#define RTC_Delay()					vTaskDelay(5)
#else
	#define RTC_Delay()					DELAY_MS(5)
#endif

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * <b>初始化实时时钟RTC</b>
  * @param  bkpKeyword		备份区关键字
  * @retval pdFALSE (0) , else return pdTRUE (1).
  * @author 		Dylan
  * @version		v1.0(2013-10-24)
  */
uint8_t BSP_RTC_Init(uint16_t bkpKeyword)
{
	RTC_InitTypeDef RTC_InitStructure;
	uint16_t  sRTCTimeout = sWHILE_LONG_TIMEOUT;
	
	//使能备份域的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	//取消备份域的写保护
	PWR_BackupAccessCmd(ENABLE);

	//读RTC备份寄存器，判断是否第一次设置
	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != bkpKeyword)
	{
		//LSE 振荡器使能
		RCC_LSEConfig(RCC_LSE_ON);

		//等待LSE 振荡器就绪
		sRTCTimeout = sWHILE_LONG_TIMEOUT;
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
		{
			RTC_Delay();
			if (sRTCTimeout--)
				return pdFALSE;
		}

		//LSE 振荡器作为RTC 时钟
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

		//RTC 时钟开启
		RCC_RTCCLKCmd(ENABLE);

		//等待RTC时钟同步完成
		sRTCTimeout = sWHILE_LONG_TIMEOUT;
		while(RTC_WaitForSynchro() == ERROR)
		{
			RTC_Delay();
			if (sRTCTimeout--)
				return pdFALSE;
		}

		//初始化RTC
		RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
		RTC_InitStructure.RTC_SynchPrediv = 0xFF;
		RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;

		sRTCTimeout = sWHILE_LONG_TIMEOUT;
		while(RTC_Init(&RTC_InitStructure) == ERROR)
		{
			RTC_Delay();
			if (sRTCTimeout--)
				return pdFALSE;
		}

		RTC_WriteBackupRegister(RTC_BKP_DR0, bkpKeyword);
	}
	else
	{
		/* Wait for RTC APB registers synchronisation */
		sRTCTimeout = sWHILE_LONG_TIMEOUT;
		while(RTC_WaitForSynchro() == ERROR)
		{
			RTC_Delay();
			if (sRTCTimeout--)
				return pdFALSE;
		}
	}

	RTC_ClearFlag(RTC_FLAG_ALRAF);

	return pdTRUE;
}

/**
  * <b>设置RTC时间</b>
  * @param  dateTime		日期结构体
  * @retval pdFALSE (0) , else return pdTRUE (1).
  * @author 		Dylan
  * @version		v1.0(2013-10-24)
  */
uint8_t BSP_RTC_SetDateTime(DateTime_TypeDef* dateTime)
{
	uint16_t  sRTCTimeout = sWHILE_LONG_TIMEOUT;
	RTC_DateTypeDef		RTC_DateStruct;
	RTC_TimeTypeDef		RTC_TimeStructure;

	RTC_DateStruct.RTC_Year = dateTime->RTC_Year;
	RTC_DateStruct.RTC_Month = dateTime->Month;
	RTC_DateStruct.RTC_Date = dateTime->Day;
	RTC_DateStruct.RTC_WeekDay = dateTime->DayOfWeek;

	RTC_TimeStructure.RTC_H12 = RTC_HourFormat_24;
	RTC_TimeStructure.RTC_Hours = dateTime->Hour;
	RTC_TimeStructure.RTC_Minutes = dateTime->Minute;
	RTC_TimeStructure.RTC_Seconds = dateTime->Second;

	sRTCTimeout = sWHILE_LONG_TIMEOUT;
	while(RTC_SetDate(RTC_Format_BIN, &RTC_DateStruct) == ERROR)
	{
		RTC_Delay();
		if (sRTCTimeout--)
			return pdFALSE;
	}

	sRTCTimeout = sWHILE_LONG_TIMEOUT;
	while(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR)
	{
		RTC_Delay();
		if (sRTCTimeout--)
			return pdFALSE;
	}

    return pdTRUE;
}

/**
  * <b>获取RTC时间</b>
  * @retval			日期结构体
  * @author 		Dylan
  * @version		v1.0(2013-10-24)
  * @category	        板级支持包BSP函数
  */
void BSP_RTC_GetDateTime(DateTime_TypeDef* dateTime)
{
	RTC_DateTypeDef		RTC_DateStruct;
	RTC_TimeTypeDef		RTC_TimeStructure;

	RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);

	dateTime->RTC_Year = RTC_DateStruct.RTC_Year;
	dateTime->Month = RTC_DateStruct.RTC_Month;
	dateTime->Day = RTC_DateStruct.RTC_Date;
	dateTime->DayOfWeek = RTC_DateStruct.RTC_WeekDay;

	dateTime->Hour = RTC_TimeStructure.RTC_Hours;
	dateTime->Minute = RTC_TimeStructure.RTC_Minutes;
	dateTime->Second = RTC_TimeStructure.RTC_Seconds;
}


