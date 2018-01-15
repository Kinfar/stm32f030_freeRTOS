/**
  ******************************************************************************
  * @file /led_driver.c
  * @author  xd.wu
  * @brief  16路LED驱动，低电平点亮。
  * @last_modify_date	2014.9.23
  ******************************************************************************
  * 
  */

/* Includes ------------------------------------------------------------------*/
#include "led_driver.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LED_TURN_ON								GPIO_ResetBits
#define LED_TURN_OFF							GPIO_SetBits

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * <b>从枚举中获取端口。</b>
  * @param value     		led序号
  * @version            v1.0(2014-9-6)
  * @author             Dylan
  */
PortCollection_TypeDef LED_GetPortCollectio(uint8_t index)
{
	/*
	 * 全局变量集合改为函数检索局部变量;
	 * 增加执行时间和RO以换取RW空间;
	 * 此次编译结果（2014.9.23）：减少 RW 128 bytes, 增加 RO 128 bytes, 增加 Code 32 bytes。
	 */
	PortCollection_TypeDef ledCollection[16] =
	{
		{SPLIT_DEFINE_PORT(GPIO_SW01),	SPLIT_DEFINE_PIN(GPIO_SW01)},
		{SPLIT_DEFINE_PORT(GPIO_SW02),	SPLIT_DEFINE_PIN(GPIO_SW02)},
		{SPLIT_DEFINE_PORT(GPIO_SW03),	SPLIT_DEFINE_PIN(GPIO_SW03)},
		{SPLIT_DEFINE_PORT(GPIO_SW04),	SPLIT_DEFINE_PIN(GPIO_SW04)},
		{SPLIT_DEFINE_PORT(GPIO_SW05),	SPLIT_DEFINE_PIN(GPIO_SW05)},
		{SPLIT_DEFINE_PORT(GPIO_SW06),	SPLIT_DEFINE_PIN(GPIO_SW06)},
		{SPLIT_DEFINE_PORT(GPIO_SW07),	SPLIT_DEFINE_PIN(GPIO_SW07)},
		{SPLIT_DEFINE_PORT(GPIO_SW08),	SPLIT_DEFINE_PIN(GPIO_SW08)},
		{SPLIT_DEFINE_PORT(GPIO_SW09),	SPLIT_DEFINE_PIN(GPIO_SW09)},
		{SPLIT_DEFINE_PORT(GPIO_SW10),	SPLIT_DEFINE_PIN(GPIO_SW10)},
		{SPLIT_DEFINE_PORT(GPIO_SW11),	SPLIT_DEFINE_PIN(GPIO_SW11)},
		{SPLIT_DEFINE_PORT(GPIO_SW12),	SPLIT_DEFINE_PIN(GPIO_SW12)},
		{SPLIT_DEFINE_PORT(GPIO_SW13),	SPLIT_DEFINE_PIN(GPIO_SW13)},
		{SPLIT_DEFINE_PORT(GPIO_SW14),	SPLIT_DEFINE_PIN(GPIO_SW14)},
		{SPLIT_DEFINE_PORT(GPIO_SW15),	SPLIT_DEFINE_PIN(GPIO_SW15)},
		{SPLIT_DEFINE_PORT(GPIO_SW16),	SPLIT_DEFINE_PIN(GPIO_SW16)}
	};

	return ledCollection[index];
}

/**
  * <b>初始化LEDs's IO口作为上拉输入。</b>
  * @version            v1.0(2014-9-6)
  * @author             Dylan
  */
void BSP_LED_Input_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	uint8_t i = 0;
	PortCollection_TypeDef portColl;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	for (i = 0; i<16; i++)
	{
		portColl = LED_GetPortCollectio(i);
		
		GPIO_InitStructure.GPIO_Pin = portColl.uGPIO_Pin;
		GPIO_Init(portColl.pGPIO_Port, &GPIO_InitStructure);
	}
}

/**
  * <b>初始化LEDs's IO口作为通用输出。</b>
  * @version            v1.0(2014-9-2)
  * @author             Dylan
  */
void BSP_LED_Output_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	uint8_t i = 0;
	PortCollection_TypeDef portColl;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	for (i = 0; i<16; i++)
	{
		portColl = LED_GetPortCollectio(i);
		
		GPIO_InitStructure.GPIO_Pin = portColl.uGPIO_Pin;
		GPIO_Init(portColl.pGPIO_Port, &GPIO_InitStructure);
		LED_TURN_OFF(portColl.pGPIO_Port, portColl.uGPIO_Pin);
	}
}

/**
  * <b>点亮LED。</b>
  * @param value     		led序号
  * @version            v1.0(2014-9-6)
  * @author             Dylan
  */
void LED_ON(uint8_t index)
{
	PortCollection_TypeDef portColl = LED_GetPortCollectio(index);
	
	LED_TURN_ON(portColl.pGPIO_Port, portColl.uGPIO_Pin);
}

/**
  * <b>熄灭LED。</b>
  * @param index     		led序号
  * @version            v1.0(2014-9-6)
  * @author             Dylan
  */
void LED_OFF(uint8_t index)
{
	PortCollection_TypeDef portColl = LED_GetPortCollectio(index);
	
	LED_TURN_OFF(portColl.pGPIO_Port, portColl.uGPIO_Pin);
}

/**
	* <b>点亮所有的LEDs。</b>
  * @version            v1.0(2014-9-2)
  * @author             Dylan
  */
void LED_AllOn(void)
{
	uint8_t i = 0;
	
	for (i = 0; i<16; i++)
	{
		LED_ON(i);
	}
}

/**
	* <b>熄灭所有的LEDs。</b>
  * @version            v1.0(2014-9-2)
  * @author             Dylan
  */
void LED_AllOff(void)
{
	uint8_t i = 0;
	
	for (i = 0; i<16; i++)
	{
		LED_OFF(i);
	}
}

/**
	* <b>霓虹灯模式1: 相间点亮LEDs。</b>
  * @param value        点亮或熄灭
  * @version            v1.0(2014-9-2)
  * @author             Dylan
  */
void LED_Neon1(uint8_t value)
{
	uint8_t i;
	
	if (value)
	{
		for (i = 0; i<16; i += 2)
		{
			LED_ON(i);
			LED_OFF(i+1);
		}	
	}
	else
	{
		for (i = 0; i<16; i += 2)
		{
			LED_ON(i+1);
			LED_OFF(i);
		}	
	}
}

/**
	* <b>霓虹灯模式2: 两两点亮LEDs。</b>
  * @param no           组号
  * @version            v1.0(2014-9-2)
  * @author             Dylan
  */
void LED_Neon2(uint8_t index)
{
	LED_AllOff();
	
	index %= 8;
	
	LED_ON(index);
	LED_ON(index+8);
}

/**
  * <b>用指示灯标示十六进制数。</b>
  * @param value     		十六进制数
  * @version            v1.0(2014-9-6)
  * @author             Dylan
  */
void LEDs_DisplayHex(uint16_t value)
{
	u8 i = 0;
	for (i = 0; i<16; i++)
	{
		if((value>>i) & 0x1)
		{
			LED_ON(i);
		}
		else
		{
			LED_OFF(i);
		}
	}
}

