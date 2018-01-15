/**
  ******************************************************************************
  * @file    SysTick/SysTick_Example/stm32f0xx_it.c 
  * @author  MCD Application Team
  * @version V0.1.0
  * @date    11-November-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include  "stm32f0xx_it.h"
#include  "FreeRTOS.h"
//#include  "usart_driver.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//extern xGlobal_TypeDef glvar;
//extern CanRxMsg RxMessage;

//#if 0
//extern u16 TimeOut;
//extern CanTxMsg TxMessage;

//u8 KeyTimeOut = 0;
//#endif
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
extern void vPortSVCHandler( void );
extern void xPortPendSVHandler( void );
extern void xPortSysTickHandler( void );

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
	vPortSVCHandler();
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
	xPortPendSVHandler();
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	xPortSysTickHandler();
}
/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                            */
/******************************************************************************/

/**
	* <b>定时器中断: 1ms</b>
  * @version		1.2014.0116
  * @author 		Dylan
  * @see                Javadoc注释/Eclipse编辑
  */
void TIM2_IRQHandler(void)
{
//  if (TIM2->SR & 0x2)
//  {
//    TIM2->SR &= 0xFFFD;//clear interrupt flag	
//		
//		/* functions for com1,com2 timeout*/
//		USART_TimeoutFunction(&glvar.com1);
//		USART_TimeoutFunction(&glvar.com2);
//		
//		#if (USE_CAN_LED == 1)
//			if (glvar.CanReadLedTimer > 0)
//			{
//				glvar.CanReadLedTimer--;
//				if (glvar.CanReadLedTimer == 0)
//				{
//					LED_OFF(glvar.CanPortNo + RX_LED_BIT);
//				}
//			}
//		
//			if (glvar.CanWriteLedTimer > 0)
//			{
//				glvar.CanWriteLedTimer--;
//				if (glvar.CanWriteLedTimer == 0)
//				{
//					LED_OFF(glvar.CanPortNo + TX_LED_BIT);
//				}
//			}
//		#endif
//		
//		#if 0
//		
//		glvar.TimerCounter++;
//		
//		if(glvar.TimerCounter == 500)
//		{
//			GPIO_SetBits(ST_MCO);
//		}
//		else if(glvar.TimerCounter >= 1000)
//		{
//			glvar.TimerCounter = 0;
//			GPIO_ResetBits(ST_MCO);
//		}
//		
//		if (TimeOut)
//			TimeOut--;
//		
//		if(KeyTimeOut)
//			KeyTimeOut--;
//		#endif
//  }
//  else//清除其他异常标记
//  {
//    TIM2->SR = 0x0000;
//  }
}

/**
  * @brief  RS485/COM1 中断: 将接收到的数据写入缓存；接收计时器触发超时后，由主程序处理。
  */
void USART1_IRQHandler(void)
{
  ///USART_IRQFunction(USART1, &glvar.com1);
}

/**
	* @brief  PL2303HX/COM2 中断: 将接收到的数据写入缓存；接收计时器触发超时后，由主程序处理。
  */
void USART2_IRQHandler(void)
{
	//USART_IRQFunction(USART2, &glvar.com2);
}

/**
  * @brief  This function handles CAN1 RX0 request.
  * @param  None
  * @retval None
  */
void CEC_CAN_IRQHandler(void)
{
//	CAN_Receive(CAN, CAN_FIFO0, &RxMessage);

//	if (RxMessage.IDE == CAN_ID_STD)
//	{
//		glvar.canIsRead = 1;
//	}
//	else if (RxMessage.IDE == CAN_ID_EXT)
//	{
//		glvar.canIsRead = 2;
//	}

//	LED_ON(glvar.CanPortNo + RX_LED_BIT);
//	glvar.CanReadLedTimer = 100;
}

#if 0





void EXTI0_1_IRQHandler(void)
{
//  if (EXTI->PR & 0x1)
//  {
//		//按键去抖
//		if (KeyTimeOut == 0)
//		{
//			RunningMode_TypeDef backMode = CurrentMode;
//			CurrentMode++;
//			
//			if(CurrentMode > CanNetworkMode)//RunningMode_TypeDef
//			{
//				CurrentMode = (RunningMode_TypeDef)0;
//			}
//			
//			switch(CurrentMode)
//			{
//				case RS485TestMode:					
//				case CanNetworkMode:
//				case Nion01Mode:
//				case Nion02Mode:				
//					if (backMode == SwitchInputMode)
//					{
//						LEDs_OutputInit();
//					}
//					LEDs_SetAll(LEDs_OFF);
//					TimeOut = 0;
//					break;
//				case SwitchInputMode:
//					if (backMode != SwitchInputMode)
//					{				
//						LEDs_InputInit();
//					}
//					break;
//				default:
//					break;
//			}
//			
//			KeyTimeOut = 50;//KeepTime
//		}
//		
//		EXTI->PR ^= 0x0000;
//  }
//  else//清除其他异常标记
//  {
//    EXTI->PR ^= 0x0000;
//  }
}


#endif
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
