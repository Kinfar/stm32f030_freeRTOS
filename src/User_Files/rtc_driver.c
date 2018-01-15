/**
  ******************************************************************************
  * @file /rtc_driver.c
  * @author  xd.wu
  * @brief  ����STM32_RTC��ʵʱʱ�ӳ���
  * RTC�����ù��̣�
  * 1. ��ѯRTOFFλ��ֱ��RTOFF��ֵ��Ϊ��1��
  * 2. ��CNFֵΪ1����������ģʽ
  * 3. ��һ������RTC�Ĵ�������д����
  * 4. ���CNF��־λ���˳�����ģʽ
  * 5. ��ѯRTOFF��ֱ��RTOFFλ��Ϊ��1����ȷ��д�����Ѿ���ɡ�
  * ����CNF��־λ�����ʱ��д�������ܽ��У��������������Ҫ3��RTCCLK���ڡ�
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
  * <b>��ʼ��ʵʱʱ��RTC</b>
  * @param  bkpKeyword		�������ؼ���
  * @retval pdFALSE (0) , else return pdTRUE (1).
  * @author 		Dylan
  * @version		v1.0(2013-10-24)
  */
uint8_t BSP_RTC_Init(uint16_t bkpKeyword)
{
	RTC_InitTypeDef RTC_InitStructure;
	uint16_t  sRTCTimeout = sWHILE_LONG_TIMEOUT;
	
	//ʹ�ܱ������ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	//ȡ���������д����
	PWR_BackupAccessCmd(ENABLE);

	//��RTC���ݼĴ������ж��Ƿ��һ������
	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != bkpKeyword)
	{
		//LSE ����ʹ��
		RCC_LSEConfig(RCC_LSE_ON);

		//�ȴ�LSE ��������
		sRTCTimeout = sWHILE_LONG_TIMEOUT;
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
		{
			RTC_Delay();
			if (sRTCTimeout--)
				return pdFALSE;
		}

		//LSE ������ΪRTC ʱ��
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

		//RTC ʱ�ӿ���
		RCC_RTCCLKCmd(ENABLE);

		//�ȴ�RTCʱ��ͬ�����
		sRTCTimeout = sWHILE_LONG_TIMEOUT;
		while(RTC_WaitForSynchro() == ERROR)
		{
			RTC_Delay();
			if (sRTCTimeout--)
				return pdFALSE;
		}

		//��ʼ��RTC
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
  * <b>����RTCʱ��</b>
  * @param  dateTime		���ڽṹ��
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
  * <b>��ȡRTCʱ��</b>
  * @retval			���ڽṹ��
  * @author 		Dylan
  * @version		v1.0(2013-10-24)
  * @category	        �弶֧�ְ�BSP����
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


