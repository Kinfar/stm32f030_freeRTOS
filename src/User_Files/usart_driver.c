/**
  ******************************************************************************
  * @file /usart_driver.c
  * @author  xd.wu
  * @brief  基于STM32_USART的串口接收处理程序。
  * @last_modify_date	2014.9.23
  ******************************************************************************
  * 
  */

/* Includes ------------------------------------------------------------------*/
#include "usart_driver.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * <b>初始化通用异步收发器。</b>
  * @param  USARTx    	串口外设
  * @param  baudrate    波特率
  * @param  parity      校验位。0 None,1 Odd, 2 Even
  * @param  length      数据位个数。带校验7 or 无校验8
  * @param  stopbits    停止位个数。1 1bit, 2 2bits, 3 1.5bits
  * @retval pdFALSE (0) , else return pdTRUE (1).
  * @version            v1.0(2013-12-5)
  * <p>重写函数的功能说明！
  * @author             Dylan
  */
uint8_t BSP_USART_Init(USART_TypeDef* USARTx, u32 baudRate, u8 parity, u8 length, u8 stopBits)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	if (USARTx == USART1)
	{
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

		// GPIO的公共特性
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

		//COM1
		//USARTx_TX   GPIO_Mode_AF_PP
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Pin = SPLIT_DEFINE_PIN(ST_COM1_Tx);
		GPIO_Init(SPLIT_DEFINE_PORT(ST_COM1_Tx) , &GPIO_InitStructure);
		SetAF_COM1_Tx;//必须选择复用功能

		//USARTx_RX   GPIO_Mode_AF_PP & IPU
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Pin = SPLIT_DEFINE_PIN(ST_COM1_Rx);
		GPIO_Init(SPLIT_DEFINE_PORT(ST_COM1_Rx), &GPIO_InitStructure);
		SetAF_COM1_Rx;//必须选择复用功能

		//RS485-DE
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Pin = SPLIT_DEFINE_PIN(ST_COM1_DE);
		GPIO_Init(SPLIT_DEFINE_PORT(ST_COM1_DE), &GPIO_InitStructure);
		GPIO_SetBits(RS485_DE);

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, ENABLE);
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, DISABLE);
	}
	else if (USARTx == USART2)
	{
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

		// GPIO的公共特性
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

		//COM1
		//USARTx_TX   GPIO_Mode_AF_PP
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Pin = SPLIT_DEFINE_PIN(ST_COM2_Tx);
		GPIO_Init(SPLIT_DEFINE_PORT(ST_COM2_Tx) , &GPIO_InitStructure);
		SetAF_COM2_Tx;//必须选择复用功能

		//USARTx_RX   GPIO_Mode_AF_PP & IPU
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Pin = SPLIT_DEFINE_PIN(ST_COM2_Rx);
		GPIO_Init(SPLIT_DEFINE_PORT(ST_COM2_Rx), &GPIO_InitStructure);
		SetAF_COM2_Rx;//必须选择复用功能

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, ENABLE);
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, DISABLE);
	}
	else
	{
		return pdFALSE;
		//#warning "Make sure USARTx is exist ?"
	}

	/* now start init usartx info. */
	switch (parity)
	{
		case 1://奇
			USART_InitStructure.USART_Parity = USART_Parity_Odd;
			if (length == 7)
				USART_InitStructure.USART_WordLength = USART_WordLength_8b;
			else
				USART_InitStructure.USART_WordLength = USART_WordLength_9b;
			break;
		case 2://偶
			USART_InitStructure.USART_Parity = USART_Parity_Even;
			if (length == 7)
				USART_InitStructure.USART_WordLength = USART_WordLength_8b;
			else
				USART_InitStructure.USART_WordLength = USART_WordLength_9b;
			break;
		default://0无8位数据位
			USART_InitStructure.USART_Parity = USART_Parity_No;
			USART_InitStructure.USART_WordLength = USART_WordLength_8b;
			break;
	}

	switch (stopBits)
	{
		case 2:
			USART_InitStructure.USART_StopBits = USART_StopBits_2;
			break;
		case 3:
			USART_InitStructure.USART_StopBits = USART_StopBits_1_5;
			break;
		#if defined  (STM32F051) || defined (STM32F042)
			//null
		#elif defined (STM32F100) || defined (STM32F101) || defined (STM32F103)
			case 0://保留STM32F05不支持,STM32F10支持
				USART_InitStructure.USART_StopBits = USART_StopBits_0_5;
				break;
		#else
			#warning "Make sure needn't define STM32F051 etc. ?"
		#endif
		default://1
			USART_InitStructure.USART_StopBits = USART_StopBits_1;
			break;
	}

	USART_InitStructure.USART_BaudRate = baudRate;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USARTx, &USART_InitStructure);

	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);

	USART_Cmd(USARTx, ENABLE);
	
	return pdTRUE;
}

/**
  * <b>串口接收使能。</b>
  * @param  COMx      	COM数据结构体
  * @version            v1.0(2014-9-10)
  * @author             Dylan
  */
void COM_ReadEnable(COM_TypeDef* COMx)
{
	COMx->IsReadTimeout = pdFALSE;
	COMx->IsReadEnable = pdFALSE;	
}

/**
  * <b>默认初始化串口。</b>
  * @param  COMx      	COM数据结构体
  * @version            v1.0(2014-9-10)
  * @author             Dylan
  */
void COM_DeInit(COM_TypeDef* COMx)
{
	uint16_t i;	
	
	COMx->ReadBufferSize = COM_BUFF_DEPTH;
	COMx->ReadTimeout = 1000;
	
	for(i=0; i<COM_BUFF_DEPTH; i++)
	{
		COMx->ReadBuffer[i] = 0x00;
	}
	
	COM_ReadEnable(COMx);
}

/**
  * <b>初始化串口缓存。</b>
  * @param  COMx      	COM数据结构体
	* @param  rxDepth     COM数据缓存深度
	* @param  rxTimeout   接收超时ms数
  * @version            v1.0(2014-9-10)
  * @author             Dylan
  */
void COM_Init(COM_TypeDef* COMx, uint8_t rxCacheDepth, uint16_t rxTimeout)
{
	uint16_t i;
	
	COMx->ReadBufferSize = rxCacheDepth;
	COMx->ReadTimeout = 1000;
	
	for(i=0; i<COMx->ReadBufferSize; i++)
	{
		COMx->ReadBuffer[i] = 0x00;
		COMx->WriteBuffer[i] = 0x00;
	}
	
	COM_ReadEnable(COMx);
}

/**
  * <b>串口中断函数：用于接收缓存存放。</b>
  * @param  USARTx      COM外设
  * @param  COMx      	COM数据结构体
  * @version            v1.0(2014-9-10)
  * @author             Dylan
  */
void USART_IRQFunction(USART_TypeDef* USARTx, COM_TypeDef* COMx)
{
	uint32_t srValue = USARTx->ISR;

	if (srValue & 0x20)
	{
		COMx->ReadByte = USARTx->RDR;
		COMx->ReadTimer = 1;

		if (COMx->IsReadEnable == pdFALSE)
		{
			COMx->BytesToRead = 0;
			COMx->IsReadTimeout = pdFALSE;
			COMx->IsReadEnable = pdTRUE;
		}

		if (COMx->BytesToRead < COMx->ReadBufferSize)
		{
			COMx->ReadBuffer[COMx->BytesToRead++] = COMx->ReadByte;
		}
		else// 缓存已满，请求主程序处理。
		{
			COMx->IsReadTimeout = pdFALSE;
		}

		#if (USE_COM_LED == 1)
			LED_ON(COMx->PortNo + RX_LED_BIT);	//接收状态指示灯---闪烁
			COMx->ReadLedTimer = 100;
		#endif
	}
	else//清除其他异常标记
	{
		USARTx->ICR = srValue & 0x00121B5F;
	}
}

/**
  * <b>串口超时函数：用于定义接收超时。</b>
  * @param  COMx      	COM数据结构体
  * @version            v1.0(2014-9-10)
  * @author             Dylan
  * @see                Javadoc注释/Eclipse编辑
  */
void USART_TimeoutFunction(COM_TypeDef* COMx)
{
	if ((COMx->IsReadTimeout == pdFALSE) && (COMx->ReadTimer > 0))
	{
		COMx->ReadTimer++;

		if (COMx->ReadTimer > COMx->ReadTimeout)
		{
			COMx->ReadTimer = 0;
			COMx->IsReadTimeout = pdTRUE;
		}
	}
	
	#if (USE_COM_LED == 1)
		if (COMx->ReadLedTimer > 0)
		{
			COMx->ReadLedTimer--;
			if (COMx->ReadLedTimer == 0)
			{
				LED_OFF(COMx->PortNo + RX_LED_BIT);
			}
		}
		
		if (COMx->WriteLedTimer > 0)
		{
			COMx->WriteLedTimer--;
			if (COMx->WriteLedTimer == 0)
			{
				LED_OFF(COMx->PortNo + TX_LED_BIT);
			}
		}
	#endif
}

/**
  * <b>串口向外发送数组。</b>
  * @param  USARTx      COM外设
  * @param  COMx      	COM数据结构体
  * @param start        数组偏移
  * @param length       数据长度
  * @version		v1.0(2013-12-5)
  * <p>重写函数的功能说明！
  * @author 		Dylan
  */
void COM_SendData(USART_TypeDef* USARTx, COM_TypeDef* COMx, uint8_t start, uint8_t length)
{
	uint16_t srValue = USARTx->ISR;

	// keep bytes's length less than buff's depth.
	if (start + length > COM_BUFF_DEPTH)
	{
		length = COM_BUFF_DEPTH - start;
	}
	
	while (length--)
	{
		//taskENTER_CRITICAL();
		
		USARTx->TDR = COMx->WriteBuffer[start++];
		
		//taskEXIT_CRITICAL();
		
		#if (USE_COM_LED == 1)
			LED_ON(COMx->PortNo + TX_LED_BIT);
			COMx->WriteLedTimer = 100;
		#endif
		
		while ((USARTx->ISR & 0x0040) == 0);
	}
}

/**
  * <b>串口向外发送数组。</b>
  * @param  USARTx      COM外设
  * @param pBuff        数组
  * @param length       数据长度
  * @version		v1.0(2013-12-5)
  * @author 		Dylan
  */
void COM_SendBytes(USART_TypeDef* USARTx, uint8_t* pBuff, uint8_t length)
{
	uint16_t srValue = USARTx->ISR;
	
	while (length--)
	{
		USARTx->TDR = *pBuff++;				
		while ((USARTx->ISR & 0x0040) == 0);
	}
}

/**
  * <b>串口间相互传递信息。</b>
  * @param  COMx_Src    COM数据结构体-发送端
  * @param  COMx_Dest   COM数据结构体-接收端
  * @param  USARTx      COM传递的媒介/外设
  * @retval 0,other			若函数条件全满足，返回1。
  * @retval pdFALSE (0) , else return pdTRUE (1).
  * @version		v1.0(2013-12-5)
  * @author 		Dylan
  */
uint8_t COM_SendEachOther(COM_TypeDef* COMx_Src, COM_TypeDef* COMx_Dest, USART_TypeDef* byUSARTx)
{
	uint8_t i, ret = pdFALSE;
	
	if (COMx_Src->IsReadTimeout != pdFALSE)
	{
		for (i=0; i<COMx_Src->ReadBufferSize; i++)
		{
			COMx_Dest->WriteBuffer[i] = COMx_Src->ReadBuffer[i];
		}

		COM_SendData(byUSARTx, COMx_Dest, 0, COMx_Src->BytesToRead);

		ret = pdTRUE;

		COM_ReadEnable(COMx_Src);
	}
	
	return ret;
}
