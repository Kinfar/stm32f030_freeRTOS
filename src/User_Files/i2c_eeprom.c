/**
  ******************************************************************************
  * @file /i2c_eeprom.c
  * @author  xd.wu
  * @brief  基于STM32_IIC读写EEPROM：Microchip 24LC02B。
  * @last_modify_date	2014.9.23
  ******************************************************************************
  * 
  */
	
/* Includes ------------------------------------------------------------------*/
#include "i2c_eeprom.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#if ( INCLUDE_vTaskDelay == 1 )
	#include "task.h"

	#define sEE_Delay()					vTaskDelay(5)
#else
	#define sEE_Delay()					DELAY_MS(5)
#endif

	#define sEE_I2C                     I2C1
	#define sEE_I2C_OWN_ADDRESS         0x10

	#define sEE_DEVICE_TYPE_IDENTIFIER	0xA0
	#define sEE_CHIP_ENABLE             0x00   /* A2 = A1 = A0 = GND */

	#define sEE_DEV_SEL                	( (uint8_t) (sEE_DEVICE_TYPE_IDENTIFIER | sEE_CHIP_ENABLE) )
	#define sEE_OK                      0
	#define sEE_FAIL                    1
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
uint8_t sEE_isBusyError(void);
uint8_t sEE_TIMEOUT_UserCallback(void);


/**
  * @brief  初始化硬件IIC
  */
void BSP_I2C1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStruct;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	// I2C1 Pin I2C时钟/数据 开漏复用输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = SPLIT_DEFINE_PIN(ST_I2C1_SCL);
	GPIO_Init(SPLIT_DEFINE_PORT(ST_I2C1_SCL), &GPIO_InitStructure);
	SetAF_I2C1_SCL;
	
	//
	GPIO_InitStructure.GPIO_Pin = SPLIT_DEFINE_PIN(ST_I2C1_SDA);
	GPIO_Init(SPLIT_DEFINE_PORT(ST_I2C1_SDA), &GPIO_InitStructure);
	SetAF_I2C1_SDA;

	// 复位I2C
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);

	// 初始化
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_AnalogFilter = I2C_AnalogFilter_Enable;    //开启模拟噪声滤波器
	I2C_InitStruct.I2C_DigitalFilter = 0x00;                      //关闭数字噪声滤波器
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStruct.I2C_OwnAddress1 = sEE_I2C_OWN_ADDRESS;//I2C1_OwnAddress1;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;//在当前字节收到后发送ACK
	/*
		详见《RM0091》528页23.4.10章之表71-73 I2Cx_TIMINGR 寄存器配置举例
	*/
	//FastMode 400kH 48MHz
	//PRESC SCL_L SCL_H SDA_DEL SCL_DEL
	/*
		时序预分频器(计数间隔)      5<<28----------48/(5+1)=8M,125ns
		数据建立时间                3<<20----------(3+1)*125=500ns
		数据保持时间                4<<16----------4*125=500ns
		SCL 高电平时间              5<<8----------(5+1)*125=750ns
		SCL 低电平时间              11<<0----------(11+1)*125=1500ns
	*/
	I2C_InitStruct.I2C_Timing = (5<<28) | (3<<20) | (4<<16) | (5<<8) | (11<<0);//I2Cx->TIMINGR寄存器
	I2C_Init(I2C1, &I2C_InitStruct);

	I2C_Cmd(I2C1, ENABLE);
}

/**
  * @brief  初始化I2C_EEPROM
  * @retval sEE_OK (0) , else return the timeout user callback.
  */
uint8_t sEE_Init(void)
{
	BSP_I2C1_Init();
	
	return sEE_OK;
}

/**
  * @brief  读I2C_EEPROM
  * @param	pDataOut
  * @param	byteAddr
  * @param	bytesToRead
  * @retval sEE_OK (0) , else return the timeout user callback.
  */
uint8_t sEE_ReadBytes(uint8_t* pDataOut, uint8_t byteAddr, uint8_t bytesToRead)
{  
	uint16_t  sEETimeout = sWHILE_LONG_TIMEOUT;
	
	// Configure slave address, nbytes, reload and generate start
	I2C_TransferHandling(sEE_I2C, sEE_DEV_SEL, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);

	// Wait until TXIS flag is set
	sEETimeout = sWHILE_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(sEE_I2C, I2C_ISR_TXIS) == RESET)
	{
		sEE_Delay();
		if((sEETimeout--) == 0)
			return sEE_TIMEOUT_UserCallback();
	}

	//Send memory address
	I2C_SendData(sEE_I2C, byteAddr);

	// Wait until TC flag is set
	sEETimeout = sWHILE_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(sEE_I2C, I2C_ISR_TC) == RESET)
	{
		sEE_Delay();
		if((sEETimeout--) == 0)
			return sEE_TIMEOUT_UserCallback();
	}

	// Update CR2 : set Slave Address , set read request, generate Start and set end mode
	I2C_TransferHandling(sEE_I2C, sEE_DEV_SEL, bytesToRead, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);

	while (bytesToRead--)
	{
		// Wait until RXNE flag is set
		sEETimeout = sWHILE_LONG_TIMEOUT;
		while(I2C_GetFlagStatus(sEE_I2C, I2C_ISR_RXNE) == RESET)
		{
			sEE_Delay();
			if((sEETimeout--) == 0)
				return sEE_TIMEOUT_UserCallback();
		}

		// Read data from RXDR
		*pDataOut++ = I2C_ReceiveData(sEE_I2C);
	}

	// Wait until STOPF flag is set
	sEETimeout = sWHILE_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(sEE_I2C, I2C_ISR_STOPF) == RESET)
	{
		sEE_Delay();
		if((sEETimeout--) == 0)
			return sEE_TIMEOUT_UserCallback();
	}

	// Clear STOPF flag
	I2C_ClearFlag(sEE_I2C, I2C_ICR_STOPCF);

	return sEE_OK;
}

/**
  * @brief  写I2C_EEPROM
  * @param	pDataIn
  * @param	byteAddr
  * @param	bytesToWrite
  * @retval sEE_OK (0) , else return the timeout user callback.
  */
uint8_t sEE_WriteBytes(uint8_t* pDataIn, uint8_t byteAddr, uint8_t bytesToWrite)
{
	uint16_t  sEETimeout = sWHILE_LONG_TIMEOUT;
	
	// Configure slave address, nbytes, reload and generate start
	I2C_TransferHandling(sEE_I2C, sEE_DEV_SEL, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);
  
	// Wait until TXIS flag is set
	sEETimeout = sWHILE_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(sEE_I2C, I2C_ISR_TXIS) == RESET)
	{
		sEE_Delay();
		if((sEETimeout--) == 0)
			return sEE_TIMEOUT_UserCallback();
	}
  
	// Send memory address
	I2C_SendData(sEE_I2C, byteAddr);

	// Wait until TCR flag is set
	sEETimeout = sWHILE_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(sEE_I2C, I2C_ISR_TCR) == RESET)
	{
		sEE_Delay();
		if((sEETimeout--) == 0)
			return sEE_TIMEOUT_UserCallback();
	}
  
	// Update CR2 : set Slave Address , set write request, generate Start and set end mode
	I2C_TransferHandling(sEE_I2C, sEE_DEV_SEL, bytesToWrite, I2C_AutoEnd_Mode, I2C_No_StartStop);

	while (bytesToWrite--)
	{
		// Wait until TXIS flag is set
		sEETimeout = sWHILE_LONG_TIMEOUT;
		while(I2C_GetFlagStatus(sEE_I2C, I2C_ISR_TXIS) == RESET)
		{
			sEE_Delay();
			if((sEETimeout--) == 0)
				return sEE_TIMEOUT_UserCallback();
		}
    
		// Write data to TXDR
		I2C_SendData(sEE_I2C, *pDataIn++);
	}
  
	// Wait until STOPF flag is set ----由I2C_AutoEnd_Mode产生
	sEETimeout = sWHILE_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(sEE_I2C, I2C_ISR_STOPF) == RESET)
	{
		sEE_Delay();
		if((sEETimeout--) == 0)
			return sEE_TIMEOUT_UserCallback();
	}
  
	// Clear STOPF flag
	I2C_ClearFlag(sEE_I2C, I2C_ICR_STOPCF);
  
	return sEE_OK;
}

/**
  * @brief  I2C硬件总线BUSY异常
  * @retval sEE_OK (0) , else return sEE_FAIL (1).
  */
uint8_t sEE_isBusyError(void)
{
	if (I2C1->ISR & I2C_ISR_BUSY)//由于插拔EEPROM时机的原因,导致I2C总线一直被拉低成BUSY状态。---- >10us
	{
		I2C_TransferHandling(sEE_I2C, sEE_DEV_SEL, 0, I2C_AutoEnd_Mode, I2C_Generate_Stop);

		sEE_Delay();

		sEE_Init();//此时需要重新初始化I2C到正常状态

		return sEE_FAIL;
	}
  
	return sEE_OK;
}

/**
  * @brief  超时后清除硬件的置位
  * @retval return sEE_FAIL (1).
  */
uint8_t sEE_TIMEOUT_UserCallback(void)
{
	/* Block communication and all processes */
	//出错清除ISR寄存器的标志位
	uint32_t sr = I2C1->ISR;
	uint8_t buf = 0;

	//event

	if (sr & I2C_IT_RXNE)
	{
		buf = I2C1->RXDR;
	}
	if (sr & I2C_IT_TXIS)
	{
		I2C1->TXDR = buf;
	}
	if (sr & I2C_IT_STOPF)
	{
		I2C1->ICR |= I2C_IT_STOPF;
	}
	if (sr & I2C_IT_TCR)
	{
		I2C1->CR2 |= 1<<16;//cr2 with nbytes!=0
	}
	if (sr & I2C_IT_TC)
	{
		//start or stop
		I2C1->CR2 |= 1<<14;
	}
	if (sr & I2C_IT_ADDR)
	{
		I2C1->ICR |= I2C_IT_ADDR;
	}
	if (sr & I2C_IT_NACKF)
	{
		I2C1->ICR |= I2C_IT_NACKF;
	}

	//error
	if (sr & I2C_IT_BERR)
	{
		I2C1->ICR |= I2C_IT_BERR;
	}
	if (sr & I2C_IT_ARLO)
	{
		I2C1->ICR |= I2C_IT_ARLO;
	}
	if (sr & I2C_IT_OVR)
	{
		I2C1->ICR |= I2C_IT_OVR;
	}
	if (sr & I2C_IT_PECERR)
	{
		I2C1->ICR |= I2C_IT_PECERR;
	}
	if (sr & I2C_IT_TIMEOUT)
	{
		I2C1->ICR |= I2C_IT_TIMEOUT;
	}
	if (sr & I2C_IT_ALERT)
	{
		I2C1->ICR |= I2C_IT_ALERT;
	}

	sEE_Delay();

	return sEE_FAIL;
	/*
	while (1)
	{
	}*/
}

