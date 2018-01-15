/**
  ******************************************************************************
  * @file /i2c_eeprom.c
  * @author  xd.wu
  * @brief  ����STM32_IIC��дEEPROM��Microchip 24LC02B��
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
  * @brief  ��ʼ��Ӳ��IIC
  */
void BSP_I2C1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStruct;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	// I2C1 Pin I2Cʱ��/���� ��©�������
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

	// ��λI2C
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);

	// ��ʼ��
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_AnalogFilter = I2C_AnalogFilter_Enable;    //����ģ�������˲���
	I2C_InitStruct.I2C_DigitalFilter = 0x00;                      //�ر����������˲���
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStruct.I2C_OwnAddress1 = sEE_I2C_OWN_ADDRESS;//I2C1_OwnAddress1;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;//�ڵ�ǰ�ֽ��յ�����ACK
	/*
		�����RM0091��528ҳ23.4.10��֮��71-73 I2Cx_TIMINGR �Ĵ������þ���
	*/
	//FastMode 400kH 48MHz
	//PRESC SCL_L SCL_H SDA_DEL SCL_DEL
	/*
		ʱ��Ԥ��Ƶ��(�������)      5<<28----------48/(5+1)=8M,125ns
		���ݽ���ʱ��                3<<20----------(3+1)*125=500ns
		���ݱ���ʱ��                4<<16----------4*125=500ns
		SCL �ߵ�ƽʱ��              5<<8----------(5+1)*125=750ns
		SCL �͵�ƽʱ��              11<<0----------(11+1)*125=1500ns
	*/
	I2C_InitStruct.I2C_Timing = (5<<28) | (3<<20) | (4<<16) | (5<<8) | (11<<0);//I2Cx->TIMINGR�Ĵ���
	I2C_Init(I2C1, &I2C_InitStruct);

	I2C_Cmd(I2C1, ENABLE);
}

/**
  * @brief  ��ʼ��I2C_EEPROM
  * @retval sEE_OK (0) , else return the timeout user callback.
  */
uint8_t sEE_Init(void)
{
	BSP_I2C1_Init();
	
	return sEE_OK;
}

/**
  * @brief  ��I2C_EEPROM
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
  * @brief  дI2C_EEPROM
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
  
	// Wait until STOPF flag is set ----��I2C_AutoEnd_Mode����
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
  * @brief  I2CӲ������BUSY�쳣
  * @retval sEE_OK (0) , else return sEE_FAIL (1).
  */
uint8_t sEE_isBusyError(void)
{
	if (I2C1->ISR & I2C_ISR_BUSY)//���ڲ��EEPROMʱ����ԭ��,����I2C����һֱ�����ͳ�BUSY״̬��---- >10us
	{
		I2C_TransferHandling(sEE_I2C, sEE_DEV_SEL, 0, I2C_AutoEnd_Mode, I2C_Generate_Stop);

		sEE_Delay();

		sEE_Init();//��ʱ��Ҫ���³�ʼ��I2C������״̬

		return sEE_FAIL;
	}
  
	return sEE_OK;
}

/**
  * @brief  ��ʱ�����Ӳ������λ
  * @retval return sEE_FAIL (1).
  */
uint8_t sEE_TIMEOUT_UserCallback(void)
{
	/* Block communication and all processes */
	//�������ISR�Ĵ����ı�־λ
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

