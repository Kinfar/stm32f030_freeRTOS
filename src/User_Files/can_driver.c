/**
  ******************************************************************************
  * @file /can_driver.c
  * @author  xd.wu
  * @brief  STM32_CAN驱动。
  * 通信距离最远可达10KM(速率低于5Kbps)速率可达到1Mbps(通信距离小40M）。
  * CAN总线传输介质可以是双绞线，同轴电缆。
  * @last_modify_date	2014.9.23
  ******************************************************************************
  * 
  */

/* Includes ------------------------------------------------------------------*/
#include "can_driver.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern xGlobal_TypeDef glvar;
extern CanRxMsg RxMessage;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * <b>唤醒CAN收发器MCP2562。</b>
  * @version            v1.0(2014-9-7)
  * @author             Dylan
  */
void BSP_MCP2562_Enable(void)
{
	GPIO_SetBits(CAN1_STBY);

	DELAY_MS(1);

	GPIO_ResetBits(CAN1_STBY);
}

/**
  * <b>挂起CAN收发器MCP2562。</b>
  * @version            v1.0(2014-9-7)
  * @author             Dylan
  */
void BSP_MCP2562_Disable(void)
{
	GPIO_ResetBits(CAN1_STBY);

	DELAY_MS(1);

	GPIO_SetBits(CAN1_STBY);
}

/**
  * <b>初始化CAN。</b>
	* @param kbpsBaudRate 波特率
  * @version            v1.0(2014-9-7)
  * @author             Dylan
  */
void BSP_CAN_Init(uint16_t kbpsBaudRate)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  
	//RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = SPLIT_DEFINE_PIN(ST_CAN1_Rx);
	GPIO_Init(SPLIT_DEFINE_PORT(ST_CAN1_Rx) , &GPIO_InitStructure);
	SetAF_CAN1_Rx;//必须选择复用功能
	
	//TX
	GPIO_InitStructure.GPIO_Pin = SPLIT_DEFINE_PIN(ST_CAN1_Tx);
	GPIO_Init(SPLIT_DEFINE_PORT(ST_CAN1_Tx) , &GPIO_InitStructure);
	SetAF_CAN1_Tx;//必须选择复用功能
	
	//CAN STBY(MCP2562)	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = SPLIT_DEFINE_PIN(CAN1_STBY);
	GPIO_Init(SPLIT_DEFINE_PORT(CAN1_STBY) , &GPIO_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN, ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN , ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN , DISABLE);

	/* CAN register init */
	CAN_DeInit(CAN);
	CAN_StructInit(&CAN_InitStructure);

	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;//CAN_Mode_LoopBack;//
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	
	/* 波特率计算方法: BaudRate = APB1/((BS1+BS2+1)*Prescaler) */
	/* CAN Baudrate = 1MBps (CAN clocked at 48 MHz) */
	switch(kbpsBaudRate)
	{
		case 10://kbps
			CAN_InitStructure.CAN_BS1 = CAN_BS1_13tq;
			CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
			CAN_InitStructure.CAN_Prescaler = 300;
			break;
		case 20://kbps
			CAN_InitStructure.CAN_BS1 = CAN_BS1_13tq;
			CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
			CAN_InitStructure.CAN_Prescaler = 150;
			break;
		case 50://kbps
			CAN_InitStructure.CAN_BS1 = CAN_BS1_13tq;
			CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
			CAN_InitStructure.CAN_Prescaler = 60;
			break;
		case 100://kbps
			CAN_InitStructure.CAN_BS1 = CAN_BS1_13tq;
			CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
			CAN_InitStructure.CAN_Prescaler = 30;
			break;
		case 125://kbps
			CAN_InitStructure.CAN_BS1 = CAN_BS1_13tq;
			CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
			CAN_InitStructure.CAN_Prescaler = 24;
			break;
		case 250://kbps
			CAN_InitStructure.CAN_BS1 = CAN_BS1_13tq;
			CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
			CAN_InitStructure.CAN_Prescaler = 12;
			break;
		case 500://kbps 500kbps 速率下，CANOpen协议推荐采样点的位置在8tq位置处，BS1=7, BS2=3
			CAN_InitStructure.CAN_BS1 = CAN_BS1_13tq;
			CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
			CAN_InitStructure.CAN_Prescaler = 6;
			break;
		default://1Mbps
			CAN_InitStructure.CAN_BS1 = CAN_BS1_5tq;
			CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
			CAN_InitStructure.CAN_Prescaler = 6;
			break;
	}
	
	CAN_Init(CAN, &CAN_InitStructure);

	/* CAN filter init */
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	/* CAN FIFO0 message pending interrupt enable */
	CAN_ITConfig(CAN, CAN_IT_FMP0, ENABLE);
	
	BSP_MCP2562_Enable();
}

