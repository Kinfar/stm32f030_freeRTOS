#include "nrf24l01.h"
#include "spi.h"
#include "delay.h"
/*
                  ////////////

            /////////////////////////                   /////////////////////////////
           ////                /////                               //////
          ////                /////                               //////
         /////               /////                  /////////////////////////////////////////
        //////////////////////////                             //////  /////
       /////                                                  //////     /////
     /////    ///////////////////                            //////        /////
    ////      ////          /////                           /////            //////
   ////       ////          /////                          /////              ///////
  ////        ////          /////                         /////                ////////
 /////        ///////////////////                        /////                   /////////
//启天科技出品
//研发团队：启天四轴团队
//联系方式：QQ群：471023785
            邮箱：qitiansizhou@163.com
            淘宝：http://shop125061094.taobao.com/
//声明：此程序只可以用于学习教学，禁止用于商业用途，否则后果必究！
//版本：V2.6
//日期：20150807
//修改说明：
//
*/

uint8_t NRF24L01_RXDATA[RX_PLOAD_WIDTH] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//nrf24l01接收到的数据
uint8_t NRF24L01_TXDATA[RX_PLOAD_WIDTH] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//nrf24l01需要发送的数据

static uint8_t TX_ADDRESS[TX_ADR_WIDTH]= {0x13,0x31,0x63,0x66,0x85};	//本地地址
static uint8_t RX_ADDRESS[RX_ADR_WIDTH]= {0x13,0x31,0x63,0x66,0x85};	//接收地址

/*
*****************************************************************
* 写寄存器
*****************************************************************
*/
uint8_t NRF_Write_Reg(uint8_t reg, uint8_t value)
{
	uint8_t status;
	SPI_CSN_L();					  
	status = SPI_RW(reg);  
	SPI_RW(value);		  /* 写数据 */
	SPI_CSN_H();					  /* 禁止该器件 */
    return 	status;
}
/*
*****************************************************************
* 读寄存器
*****************************************************************
*/
uint8_t NRF_Read_Reg(uint8_t reg)
{
	uint8_t reg_val;
	SPI_CSN_L();					 
	SPI_RW(reg);			  
	reg_val = SPI_RW(0);	  /* 读取该寄存器返回数据 */
	SPI_CSN_H();					  /* 禁止该器件 */
    return 	reg_val;
}
/*
*****************************************************************
* 检测NRF是否正常，向nRF的写寄存器写入buf，然后读出来，如果nRF不正常，则无法读出或者读出数据不正确。
*****************************************************************
*/    
u8 NRF_CHECK(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 i;
	      
	NRF_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);  
	buf[0] = 0;
	NRF_Read_Buf(TX_ADDR,buf,5); 
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;                                                                   
	if(i!=5)return 0;     
	return 1;                
}
/*
*****************************************************************
*
* 写缓冲区
*
*****************************************************************
*/
uint8_t NRF_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uchars)
{
	uint8_t i;
	uint8_t status;
	SPI_CSN_L();				        /* 选通器件 */
	status = SPI_RW(reg);	/* 写寄存器地址 */
	for(i=0; i<uchars; i++)
	{
		SPI_RW(pBuf[i]);		/* 写数据 */
	}
	SPI_CSN_H();						/* 禁止该器件 */
    return 	status;	
}
/*
*****************************************************************
* 读缓冲区
*****************************************************************
*/
uint8_t NRF_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uchars)
{
	uint8_t i;
	uint8_t status;
	SPI_CSN_L();						/* 选通器件 */
	status = SPI_RW(reg);	/* 写寄存器地址 */
	for(i=0; i<uchars; i++)
	{
		pBuf[i] = SPI_RW(0); /* 读取返回数据 */ 	
	}
	SPI_CSN_H();						/* 禁止该器件 */
    return 	status;
}

/******************************************************************
* 写数据包
******************************************************************/
void NRF_TxPacket(uint8_t * tx_buf, uint8_t len)
{	
	SPI_CE_L();		 //StandBy I模式	
	NRF_Write_Buf(WR_TX_PLOAD, tx_buf, len); 			 // 装载数据	
	SPI_CE_H();		 //置高CE，激发数据发送
}
void NRF24L01_INIT(void)
{
	//这部分已经在SPI中初始化了
//	GPIO_InitTypeDef GPIO_InitStructure;

//  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
//	//24L01的IO初始化
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
	SPI_CE_L();		 //StandBy I模式	
	SPI_CE_L();
	NRF_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH);//写RX节点地址 
	NRF_Write_Buf(NRF_WRITE_REG+TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH); //写TX节点地址  
	NRF_Write_Reg(NRF_WRITE_REG+EN_AA,0x01); //使能通道0的自动应答 
	NRF_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);//使能通道0的接收地址 
	NRF_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次 
	NRF_Write_Reg(NRF_WRITE_REG+RF_CH,40); //设置RF通道为CHANAL
	NRF_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度 
	NRF_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x26); //设置TX发射参数,0db增益,2Mbps,低噪声增益开启

	NRF_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0e);   		 // IRQ收发完成中断响应，16位CRC，主发送
	
	SPI_CE_H();
}
/*
*****************************************************************
* 接收模式
*****************************************************************
*/
void SetRX_Mode(void)
{
	SPI_CE_L();
	NRF_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0f);   		// IRQ收发完成中断响应，16位CRC	，主接收
	SPI_CE_H();
} 
/*
*****************************************************************
* 发送模式
*****************************************************************
*/
void SetTX_Mode(void)
{
	SPI_CE_L();
	NRF_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0e);   		 // IRQ收发完成中断响应，16位CRC，主发送
	SPI_CE_H();
} 
/*======================================================================*/
void NRF_IRQ(void)
{
	uint8_t sta = NRF_Read_Reg(NRF_READ_REG + NRFRegSTATUS);
	if(sta & (1<<RX_DR))//接收中断
	{
	}
	if(sta & (1<<TX_DS))//发送完成并接收到应答信号中断
	{
		NRF_Write_Reg(0x27, 0xff);//清除nrf的中断标志位
	}
	if(sta & (1<<MAX_RT))//达到最大重发次数中断
	{
		NRF_Write_Reg(0x27, 0xff);//清除nrf的中断标志位
	}
}
void NRF_GPIO_Interrupt_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line12) != RESET)
	{
			EXTI_ClearITPendingBit(EXTI_Line12);
	}
}

