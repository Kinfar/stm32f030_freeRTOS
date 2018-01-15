//程序检查完成时间：2013/1/17
/**
  ******************************************************************************
  * @file /src/main.c
  * @author  xd.wu
  * @brief  Main program body
  ******************************************************************************
  *
  */

/* Includes ------------------------------------------------------------------*/
#include  "FreeRTOS.h"
#include  "rtc_driver.h"
#include  "led_driver.h"
#include  "usart_driver.h"
#include  "can_driver.h"
#include  "timers.h"
#include  "HardwareSTM32.h"
#include  "Software.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
xGlobal_TypeDef glvar;
CanRxMsg RxMessage;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void CAN_SendMsg(u8 enable);
void COM_WriteEEPROM(u8 enable);

/** @brief RTOS Tasks
  * @{
  */
	
/**
  * @brief  RTOS心跳指示任务
  * @param  None
  * @retval None
  */
void RTOSHeartbeat_Task(void* pvParam)
{
	portTickType xLastWakeTime;	
	
	GPIO_InitTypeDef	GPIO_InitStructure;
	
	//GPIOA
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
	//输入引脚 RUN_LED
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = SPLIT_DEFINE_PIN(ST_MCO);
	GPIO_Init(SPLIT_DEFINE_PORT(ST_MCO), &GPIO_InitStructure);
	
	xLastWakeTime = xTaskGetTickCount();
	
	while(1)
	{
		if (GPIO_ReadOutputDataBit(ST_MCO))
		{
			GPIO_ResetBits(ST_MCO);
		}
		else
		{
			GPIO_SetBits(ST_MCO);
		}
		
		vTaskDelayUntil(&xLastWakeTime, 500 / portTICK_RATE_MS);
	}
}

/**
  * @brief  霓虹灯任务
  * @param  None
  * @retval None
  */
void Neon_Task(void* pvParam)
{
	portTickType xLastWakeTime;
	
	uint8_t param = 0;	
	
	BSP_LED_Output_Init();
	
	xLastWakeTime = xTaskGetTickCount();
	
	while(1)
	{
		param = ! param;
		
		LED_Neon1(param);
		
		vTaskDelayUntil(&xLastWakeTime, 250 / portTICK_RATE_MS);
	}
}

/**
  * @brief  RTC任务
  * @param  None
  * @retval None
  */
void RTC_Task(void* pvParam)
{
	DateTime_TypeDef dateTime;
	//uint8_t utc_Time[20] = "2014-01-01 01:02:03";

	dateTime.Year = 2014;
	dateTime.RTC_Year = 0;
	dateTime.Month = 10;
	dateTime.Day = 1;
	dateTime.Hour = 8;
	dateTime.Minute = 30;
	dateTime.Minute = 10;
	
	vTaskDelay(1000);

	if (BSP_RTC_Init(0xABCD) == pdTRUE)
	{		
		BSP_RTC_SetDateTime(&dateTime);
		
		while(1)
		{
			vTaskDelay(1000);
			
			BSP_RTC_GetDateTime(&dateTime);

			glvar.utc_Time[0] = ( dateTime.Year + dateTime.RTC_Year ) / 1000 + '0';
			glvar.utc_Time[1] = ( ( dateTime.Year + dateTime.RTC_Year ) / 100 ) %10 + '0';
			glvar.utc_Time[2] = ( ( dateTime.Year + dateTime.RTC_Year ) / 10) %10 + '0';
			glvar.utc_Time[3] = ( dateTime.Year + dateTime.RTC_Year ) % 10 + '0';
			glvar.utc_Time[4] = '-';
			glvar.utc_Time[5] = ( dateTime.Month / 10) % 10 + '0';
			glvar.utc_Time[6] = dateTime.Month % 10 + '0';
			glvar.utc_Time[7] = '-';
			glvar.utc_Time[8] = ( dateTime.Day / 10 ) % 10 + '0';
			glvar.utc_Time[9] = dateTime.Day % 10 + '0';
			glvar.utc_Time[10] = ' ';
			glvar.utc_Time[11] = ( dateTime.Hour / 10 ) % 10 + '0';
			glvar.utc_Time[12] = dateTime.Hour % 10 + '0';
			glvar.utc_Time[13] = ':';
			glvar.utc_Time[14] = ( dateTime.Minute / 10 ) % 10 + '0';
			glvar.utc_Time[15] = dateTime.Minute % 10 + '0';
			glvar.utc_Time[16] = ':';
			glvar.utc_Time[17] = ( dateTime.Second / 10 ) % 10 + '0';
			glvar.utc_Time[18] = dateTime.Second % 10 + '0';
			glvar.utc_Time[19] = '\0';

			//COM_SendBytes(USART2, utc_Time, 20);
		}
	}
	
	vTaskDelete( NULL );
}

/**
  * @brief  USART1任务
  * @param  None
  * @retval None
  */
void COM1_Task(void* pvParam)
{	
	glvar.com1.PortNo = 0;
	glvar.com1.ReadTimeout = 10;

	while(1)
	{
		COM_SendEachOther(&glvar.com1, &glvar.com2, USART2);
	
		vTaskDelay(100);
	}
}

/**
  * @brief  USART2任务
  * @param  None
  * @retval None
  */
void COM2_Task(void* pvParam)
{
	CanTxMsg TxMessage;
	uint8_t i, j;
	uint8_t can_ID[6];
	uint8_t enable;
	
	glvar.com2.PortNo = 2;
	glvar.com2.ReadTimeout = 10;

	while(1)
	{
		enable = COM_SendEachOther(&glvar.com2, &glvar.com1, USART1);
		
		if(enable)
		{
			TxMessage.StdId = 0x00;
			TxMessage.ExtId = 0x1234;
			TxMessage.IDE = CAN_ID_EXT;
			TxMessage.RTR = CAN_RTR_DATA;

			for (i=0; i<glvar.com2.BytesToRead; i++)
			{
				for (j=0; j<8; j++)
				{
					TxMessage.Data[j] = glvar.com2.ReadBuffer[i++];
					TxMessage.DLC = j;
					if (i > glvar.com2.BytesToRead)
						break;
				}
				CAN_Transmit(CAN, &TxMessage);
				LED_ON(glvar.CanPortNo + TX_LED_BIT);
				glvar.CanWriteLedTimer = 100;
			}
		}

		if (glvar.canIsRead != 0)
		{
			glvar.canIsRead = 0;

			can_ID[0] = (RxMessage.StdId >> 8) & 0xFF;
			can_ID[1] = RxMessage.StdId & 0xFF;
			can_ID[2] = (RxMessage.ExtId >> 24) & 0xFF;
			can_ID[3] = (RxMessage.ExtId >> 16) & 0xFF;
			can_ID[4] = (RxMessage.ExtId >> 8) & 0xFF;
			can_ID[5] = RxMessage.ExtId & 0xFF;

			COM_SendBytes(USART2, can_ID, 6);

			COM_SendBytes(USART2, RxMessage.Data, RxMessage.DLC);
		}

		vTaskDelay(100);
	}
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */ 
int main(void)
{
	BaseType_t TaskStatus = pdFALSE;
	
	BSP_Hardware_Init();
	
	BSP_LED_Output_Init();

	if (SysTick_Config(SystemCoreClock / 1000))
	{
		/* Capture error */
		while (1);
	}

	/* Create tasks */
	TaskStatus = xTaskCreate(RTOSHeartbeat_Task, (char const*)"HEART_BEAT", 100, NULL, 0, NULL);
	TaskStatus = xTaskCreate(RTC_Task, (char const*)"RTC_CALENDAR", 100, NULL, 1, NULL);
	//TaskStatus = xTaskCreate(Neon_Task, (char const*)"Neon", 100, NULL, 1, NULL);
	TaskStatus = xTaskCreate(COM1_Task, (char const*)"USART1", 100, NULL, 2, NULL);
	TaskStatus = xTaskCreate(COM2_Task, (char const*)"USART2", 100, NULL, 3, NULL);


	if (TaskStatus == pdTRUE)
	{
		/* Start the FreeRTOS scheduler */
		vTaskStartScheduler();
	}
	
	/* 如果一切正常，main()函数不应该会执行到这里。但如果执行到这里，很可能是内存堆空间不足导致空闲
	任务无法创建。第五章有讲述更多关于内存管理方面的信息 */
	for(;;);
}

#ifdef  USE_FULL_ASSERT
/** 把#ifdef DEBUG替换为#ifdef USE_FULL_ASSERT
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  //#ifdef __DEBUG_Example
    //printf("Wrong parameters value: file %s on line %d\r\n", file, line);
  //#else
    /* Infinite loop */
    while (1)
    {
    }
  //#endif
}
#endif
/**
  * @}
  */


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
