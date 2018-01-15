/**
  ******************************************************************************
  * @file /Hardware.c
  * @author  xd.wu
  * @brief  Ӳ���ӿ�����
  ******************************************************************************
  * 
  */

/* Includes ------------------------------------------------------------------*/
#include  "HardwareSTM32.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
	void SetUnusedGPIOsToEMCMode(void);
	void OpenIWDG(u8 pr, u16 rlr);
	void onPowerOnClock(void);

/** <summary>
  * ��ʼ����ͨ������/������š�
  * </summary>
  * <param name=""></param>
  * <exception cref=""></exception>
  */
void connectPinsToCorrectPosition(void)//@ok
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	SetUnusedGPIOsToEMCMode();

#if defined  (STM32F051)
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
	// GPIO�Ĺ�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	//�������� KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = SPLIT_DEFINE_PIN(ST_KEY_ONOFF);
	GPIO_Init(SPLIT_DEFINE_PORT(ST_KEY_ONOFF), &GPIO_InitStructure);
	
	//�������� KEY2
	GPIO_InitStructure.GPIO_Pin = SPLIT_DEFINE_PIN(ST_KEY_SET);
	GPIO_Init(SPLIT_DEFINE_PORT(ST_KEY_SET), &GPIO_InitStructure);
	
	//�������� KEY3
	GPIO_InitStructure.GPIO_Pin = SPLIT_DEFINE_PIN(ST_KEY3);
	GPIO_Init(SPLIT_DEFINE_PORT(ST_KEY3), &GPIO_InitStructure);
	
	//�������� KEY4
	GPIO_InitStructure.GPIO_Pin = SPLIT_DEFINE_PIN(ST_KEY_UP);
	GPIO_Init(SPLIT_DEFINE_PORT(ST_KEY_UP), &GPIO_InitStructure);
	
	//�������� KEY5
	GPIO_InitStructure.GPIO_Pin = SPLIT_DEFINE_PIN(ST_KEY_DOWN);
	GPIO_Init(SPLIT_DEFINE_PORT(ST_KEY_DOWN), &GPIO_InitStructure);
	
#elif  defined (STM32F042)
	//GPIOA
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
	//�������� RUN_LED
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = SPLIT_DEFINE_PIN(ST_MCO);
	GPIO_Init(SPLIT_DEFINE_PORT(ST_MCO), &GPIO_InitStructure);

#else
	#warning "Make sure needn't define STM32F051 etc. ?"
#endif
}

/**
  * <b>���ô�������ӿڡ�</b>
  * <p>���ܣ���ʼ��SPI1���á�
  * <p>ע�⣺STM32F0xϵ�еĽ����ж���Ҫͨ��SPI_RxFIFOThresholdConfig()��ָ�������жϵ�����������
  * @param  BaudRatePCLKDiv     ͨ��������ص�Ԥ��Ƶ��SPI_BaudRatePrescaler_16 ��APB2ʱ�����
  * @param  CPOL                ʱ�Ӽ��ԣ���������ʱ��SCK�ĵ�ƽ״̬��SPI_CPOL_Low
  * @param  CPHA_Edge           ʱ����λ���������ݲ����ı��أ����������½��ء�SPI_CPHA_1Edge
  * @version		        v1.0(2013-12-5)
  * <p>��д�����Ĺ���˵����
  * @author 		        Dylan
  * @see                        Javadocע��/Eclipse�༭
  */
void connectSPI1ToExt(u16 BaudRatePCLKDiv, u16 CPOL, u16 CPHA_Edge)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

#if  defined (STM32F042)
	//GPIOA
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  //SPIx_SCK ��ģʽ ���츴�����
  //SPIx_MOSI ȫ˫��ģʽ/��ģʽ ���츴�����
  //SPIx_MISO ȫ˫��ģʽ/��ģʽ ������������������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

  GPIO_InitStructure.GPIO_Pin = SPLIT_DEFINE_PIN(ST_SPI1_SCK);
  GPIO_Init(SPLIT_DEFINE_PORT(ST_SPI1_SCK), &GPIO_InitStructure);
  GPIO_ResetBits(ST_SPI1_SCK);
	SetAF_SPI1_SCK;
	
	GPIO_InitStructure.GPIO_Pin = SPLIT_DEFINE_PIN(ST_SPI1_MOSI);
  GPIO_Init(SPLIT_DEFINE_PORT(ST_SPI1_MOSI), &GPIO_InitStructure);
	SetAF_SPI1_MOSI;
	
  //����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  
  GPIO_InitStructure.GPIO_Pin = SPLIT_DEFINE_PIN(ST_SPI1_MISO);
  GPIO_Init(SPLIT_DEFINE_PORT(ST_SPI1_MISO), &GPIO_InitStructure);
	SetAF_SPI1_MISO;
#else
	#warning "Make sure needn't define STM32F051 etc. ?"
#endif
	
	//
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, DISABLE);

  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = CPOL;
  SPI_InitStructure.SPI_CPHA = CPHA_Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = BaudRatePCLKDiv;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);
  SPI_CalculateCRC(SPI1,DISABLE);
  
  //1: ���FIFO �Ĵ洢ˮƽ���ڻ����1/4��8 λ��������RXNE �¼���
  SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF);
  
  SPI_Cmd(SPI1, ENABLE);
}


/**
  * <b>����ͨ���첽�շ�����</b>
  * <p>���ܣ���ʼ��UART2���á�
  * @param  baudrate    ������
  * @param  parity      У��λ��0 None,1 Odd, 2 Even
  * @param  length      ����λ��������У��7 or ��У��8
  * @param  stopbits    ֹͣλ������1 1bit, 2 2bits, 3 1.5bits
  * @version            v1.0(2014-9-1)
  * <p>��д�����Ĺ���˵����
  * @author             Dylan
  * @see                Javadocע��/Eclipse�༭
  */
void connectUsart2ToExt(u32 baudrate, u8 parity, u8 length, u8 stopbits)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;  
	
#if  defined (STM32F042)
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
	// GPIO�Ĺ�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	//COM1
	//USARTx_TX   GPIO_Mode_AF_PP
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = SPLIT_DEFINE_PIN(ST_COM2_Tx);
	GPIO_Init(SPLIT_DEFINE_PORT(ST_COM2_Tx) , &GPIO_InitStructure);
	SetAF_COM2_Tx;//����ѡ���ù���

	//USARTx_RX   GPIO_Mode_AF_PP & IPU
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = SPLIT_DEFINE_PIN(ST_COM2_Rx);
	GPIO_Init(SPLIT_DEFINE_PORT(ST_COM2_Rx), &GPIO_InitStructure);
	SetAF_COM2_Rx;//����ѡ���ù���
	
#else
	#warning "Make sure needn't define STM32F051 etc. ?"
#endif

  USART_Cmd(USART2, DISABLE); //������������USART2���ȹش���

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, DISABLE);
  
  USART_InitStructure.USART_BaudRate = baudrate;
  
  switch (parity)
  {
  case 1://��
    USART_InitStructure.USART_Parity = USART_Parity_Odd;
    if (length == 7)
      USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    else
      USART_InitStructure.USART_WordLength = USART_WordLength_9b;
    break;
  case 2://ż
    USART_InitStructure.USART_Parity = USART_Parity_Even;
    if (length == 7)
      USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    else
      USART_InitStructure.USART_WordLength = USART_WordLength_9b;
    break;
  default://0��8λ����λ
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    break;
  }

  switch (stopbits)
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
  case 0://����STM32F05��֧��,STM32F10֧��
	USART_InitStructure.USART_StopBits = USART_StopBits_0_5;
	break;
#else
	#warning "Make sure needn't define STM32F051 etc. ?"
#endif
  default://1
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    break;
  }
  
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure);
  
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USART2, USART_IT_TXE, DISABLE);

  USART_Cmd(USART2, ENABLE);
}

/**
  * <b>����ͨ�ö�ʱ����</b>
  * <p>���ܣ���ʼ��TIM2-CH2���á�
  * @param  TIMx                ��ʱ��x
  * @param  SystemCLK           ϵͳ����ʱ��
  * @param  LoadingValue        ����ֵ�����Ǵ����жϵ�ʱ������
  * @version		1.1.2014.0228
  * <p>ʵ��CM0��CM3������ɣ�
  * @author                     Dylan
  * @see                        Javadocע��/Eclipse�༭
  */
void connectTimBase1US(TIM_TypeDef* TIMx, u8 SystemCLK, u16 LoadingValue)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
#if defined  (STM32F051) || defined  (STM32F042) 
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	//GPIO_Mode_AF_PP	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = SPLIT_DEFINE_PIN(ST_TIM2_CH1);
	GPIO_Init(SPLIT_DEFINE_PORT(ST_TIM2_CH1), &GPIO_InitStructure);
	SetAF_TIM2_CH1;//����ѡ���ù���

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2, DISABLE);

	// TIMx�Ĵ�������
	TIMx->PSC = SystemCLK -1;
	TIMx->ARR = LoadingValue-1;

	TIMx->CR1 = 0x0000;

	TIMx->CCMR1 &= 0xFF00;//CH1����ģʽ,�����
	//TIMx->CCMR1 |= 0x0000;

	TIMx->CCER &= 0xFFF0;
	//TIMx->CCER |= 0x0000;

	TIMx->CCR1 = TIMx->ARR;//�����������ֵARR

	TIMx->DIER |= 0x0002;//Timx ccr1 iqr enable

	TIMx->CR1 |= 0x1;//timx enable
#else
	#warning "Make sure needn't define STM32F051 etc. ?"
#endif
}

/**
  * <b>ϵͳ�δ�����ѭ����ȷ��ʱ������</b>
  * <p>���ܣ���ȷ��΢����ʱ��ÿ��һ΢�������һ��
  * @param  nus         ��Ҫ��ʱ��΢����
  * @param  HCLK        ϵͳ����ʱ��
  * @version            v1.0(2013-12-5)
  * <p>��д�����Ĺ���˵����
  * @author             Dylan
  * @see                Javadocע��/Eclipse�༭
  */
void Delay_us(u32 nus, u8 HCLK)
{  
  SysTick->CTRL &= 0x00010006;

  //(nus*HCLK/8)&0x00FFFFFF; //�ȶ�����Ŵ�8�����������ֵ����ȡ��Ч��Χ���ٽ������С��ԭ����ֵ��
  SysTick->LOAD = ((nus*HCLK)&0x07FFFFF8)>>3; //Max=(2^24)/HCLK=16.7s/HCLK

  SysTick->VAL = 0x00;  //��ռ�������ǰֵ
  
  SysTick->CTRL |= 0x00001; //ʹ�ܼ�����
  
  while((SysTick->CTRL & 0x10001) == 0x1);//����������ʹ��״̬�£�ֱ����CountFlag=1
  
  SysTick->CTRL &= 0x00010006;  //��λ,�رռ�����
}

/**
  word ��,byte �ֽ�,bit λ
  һ��ȡĩβ8ҳ��ַ��ÿҳ1KB
  ��ַ���� = (��¼�����KB+1)*0x0400 +(0x0800 0000)
  ���ҵ�ַ��������Χ��
  ����洢������
  4 = 16K�ֽڵ�����洢��
  6 = 32K�ֽڵ�����洢��
  8 = 64K�ֽڵ�����洢��
  B = 128K�ֽڵ�����洢��
  C = 256K�ֽڵ�����洢��
  D = 384K�ֽڵ�����洢��
  E = 512K�ֽڵ�����洢��
  //���磬32K��ַ��Χ0x0800 0000~0x0800 7FFF
  ��������Ʒ��д��������ÿ2ҳΪ��λʵ�ֵģ�ʣ�µĴ洢����(��62ҳ����255ҳ)����ͬʱ�ṩ������
  С�������������Ĳ�Ʒ��д��������ÿ4ҳΪ��λʵ�ֵġ�
*/
/*-------  ����ĺ� FlashSizeKB��ProgramSizeKB ***����Ҫ�����õ�Flashʱ��ҪУ�� -------*/
  #define FlashSizeKB     Current_FlashSizeKB//64  //Flash����
  #define ProgramSizeKB   Current_ProgramSizeKB//14  //��������
  #define FlashStartAddr  ((u32)(0x08000000 + (ProgramSizeKB<<10)))  //������һҳ��ʼ��ַ
  #define FlashEndAddr    ((u32)(0x08000000 + (FlashSizeKB<<10)-1))  //Flash������ַ
  #define FlashWRPR_Value ((u32)(0xFFFFFFFF << (u8)((ProgramSizeKB>>2) + 1)))//4ҳ

/**
  * <b>���ݱ��浽Flash�ĺ�����</b>
  * <p>���ܣ���Flash��ַҳ��д�뵥�֣�16λ��
  * @param  addr        ��д���Ŀ�ĵ�ַ
  * @param  data        ��д�������
  * @return             �Ƿ�ɹ�д�������
  * @version		1.1.2014.0228
  * <p>ʵ��CM0��CM3������ɣ�
  * @author             Dylan
  * @see                Javadocע��/Eclipse�༭
  */
bool Flash_wWord(u32 addr, u16 data)
{
	u32 Protected;
	FLASH_Status  FLASHStatus;
	
  //��ַ��Ч��
  if ((addr >= FlashStartAddr) && (addr <= FlashEndAddr))
  {
    //CR_LOCKλ=1ִ�н�������
    FLASH->KEYR = 0x45670123; 
    FLASH->KEYR = 0xCDEF89AB;
    
    // Clear All flags
    FLASH->SR = 0x35;
    
    //��ַ����ҳ��Flashҳ�������Ƿ����д����
    Protected = (FLASH->WRPR) & FlashWRPR_Value;
  
    if (Protected == 0x00)  //д����
    {
#if defined  (STM32F051) || defined  (STM32F042)
      FLASH_OB_Erase();//Erases the FLASH option bytes.
#elif defined (STM32F100) || defined (STM32F101) || defined (STM32F103)
      FLASH_EraseOptionBytes();//Erases the FLASH option bytes.
#else
	#warning "Make sure needn't define STM32F051 etc. ?"
#endif
      /* ---------------------------------------------------------------------------------------
      Ҫ������Կ�� 0x05FA0000 �����ǵĲ��� SYSRESETREQ[2]=1 ���,һ��д�� SCB->AIRCR,�ű�CM3����
      //NVIC_GenerateSystemReset();
      --------------------------------------------------------------------------------------- */
      SCB->AIRCR = (u32)0x05FA0000 | (u32)0x04;//�ȼ���Կ��(u32)0x05FA0000,Ȼ�����һ��ϵͳ��λ
    }
    else
    {
      // Clear All flags
      FLASH->SR = 0x35;
  
      //����ָ��ҳ FLASH pages
      FLASHStatus = FLASH_ErasePage(addr);  //�޸������ַ�Ĵ���FLASH_AR����ѡ��Ҫ������ҳ
      
      if (FLASHStatus == FLASH_COMPLETE)
        FLASHStatus = FLASH_ProgramHalfWord(addr, data);

      //��ȡд������ݣ��ȶ�
      if((*(vu16*) addr) == data)
        return TRUE;
    }
  }
  return FALSE;
}

/**
  * <b>���鱣�浽Flash�ĺ�����</b>
  * <p>���ܣ���Flash��ַҳ��д�뵥�֣�16λ��
  * @param  addr        ��д���Ŀ�ĵ�ַ
  * @param  pData       ��д�������ָ��
  * @param  len         ��д������ݳ���
  * @return             �Ƿ�ɹ�д�������
  * @version		1.1.2014.0228
  * <p>ʵ��CM0��CM3������ɣ�
  * @author             Dylan
  * @see                Javadocע��/Eclipse�༭
  */
bool Flash_wWords(u32 addr, u16 *pData, u16 len)
{
	u32 ProtectedPages;
	FLASH_Status FLASHStatus;
	u16 i;
	
  //��ַ��Ч��
  if ((addr >= FlashStartAddr) && ((addr+(len<<1)) <= FlashEndAddr))
  {
    //CR_LOCKλ=1ִ�н�������
    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xCDEF89AB;
    
    // Clear All flags
    FLASH->SR = 0x35;
    
    //��ַ����ҳ��Flashҳ�������Ƿ����д����
    ProtectedPages = (FLASH->WRPR) & FlashWRPR_Value;
  
    if (ProtectedPages == 0x00) //д����
    {
#if defined  (STM32F051) || defined  (STM32F042)
      FLASH_OB_Erase();//Erases the FLASH option bytes.
#elif defined (STM32F100) || defined (STM32F101) || defined (STM32F103)
      FLASH_EraseOptionBytes();//Erases the FLASH option bytes.
#else
	#warning "Make sure needn't define STM32F051 etc. ?"
#endif

      SCB->AIRCR = (u32)0x05FA0000 | (u32)0x04;//�ȼ���Կ��(u32)0x05FA0000,Ȼ�����һ��ϵͳ��λ
    }
    else
    {
      // Clear All flags
      FLASH->SR = 0x35;
  
      //����ָ��ҳ FLASH pages
      FLASHStatus = FLASH_ErasePage(addr);
      
      for (i=0; i<len; i++)
      {
        if (FLASHStatus == FLASH_COMPLETE)
          FLASHStatus = FLASH_ProgramHalfWord(addr + (i<<1), pData[i]);
      }
  
      for (i=0; i<len; i++)
      {
        if((*(vu16*) (addr + (i<<1))) != pData[i])
          return FALSE;
      }
      return TRUE;
    }
  }
  return FALSE;
}

/**
  * <b>����ϵͳ��ʱ�ӡ�</b>
  * <p>���ܣ�ѡ��ϵͳʱ��Դ��ϵͳʱ��Ƶ�ʡ�
  * @param  oscSelect   ʱ��Դѡ���ⲿ8M���پ���HSE ���ڲ�8M���پ���HSI
  * @param  clkMHz      оƬ�����Ƶ��M����ѯ��Ӧ�������ֲ�
  * @version		1.1.2014.0228
  * <p>ʵ��CM0��CM3������ɣ�
  * @author             Dylan
  * @see                Javadocע��/Eclipse�༭
  */
void createMasterClock(uint8_t oscSource, u8 clkMHz)
{
  onPowerOnClock();
  
  if (oscSource == USE_EXTERNAL_HSE)
  {
#if defined  (STM32F051) || defined  (STM32F042)
    RCC->CR |= 1<<16;       //HSEON
    while(((RCC->CR>>17) & 0x1) != 1);  //�ȴ�HSE����
    
    RCC->CFGR |= (clkMHz/8-2)<<18; //����PLLֵ��Ƶ//HSE 8MHz
    
    RCC->CFGR |= 1<<16; //�л�PLL����ʱ��ԴΪ0->1��HSI/2(4MHz)->HSE(8MHz)
    
    //���ڵȴ�״̬ʱ�������ȿ���Ԥȡ������
    if (clkMHz <= 24)
      FLASH->ACR |= 0x30;//000����ȴ�״̬���� 0 < SYSCLK �� 24MHz
    else if (clkMHz <= 48)
      FLASH->ACR |= 0x31;//001��һ���ȴ�״̬���� 24MHz < SYSCLK �� 48MHz
    else
      FLASH->ACR |= 0x32;//010�������ȴ�״̬���� 48MHz < SYSCLK �� 72MHz
    
    RCC->CR |= 1<<24;                   //PLLON
    while(((RCC->CR>>25) & 0x1) != 1);  //�ȴ�PLL����
    RCC->CFGR |= 0x2;                   //PLL��ΪsysClk
    while(((RCC->CFGR>>2) & 0x3) != 0x02);  //�ȴ�sysClk����
#elif defined (STM32F100) || defined (STM32F101) || defined (STM32F103)
    RCC->CR |= 1<<16;       //HSEON
    while(((RCC->CR>>17) & 0x1) != 1);  //�ȴ�HSE����
    
    RCC->CFGR |= (clkMHz/8-2)<<18; //����PLLֵ��Ƶ//HSE 8MHz
    
    RCC->CFGR |= 1<<16; //�л�PLL����ʱ��ԴΪ0->1��HSI/2(4MHz)->HSE(8MHz)
    
    //���ڵȴ�״̬ʱ�������ȿ���Ԥȡ������
    if (clkMHz <= 24)
      FLASH->ACR |= 0x30;//000����ȴ�״̬���� 0 < SYSCLK �� 24MHz
    else if (clkMHz <= 48)
      FLASH->ACR |= 0x31;//001��һ���ȴ�״̬���� 24MHz < SYSCLK �� 48MHz
    else
      FLASH->ACR |= 0x32;//010�������ȴ�״̬���� 48MHz < SYSCLK �� 72MHz
    
    RCC->CR |= 1<<24;                   //PLLON
    while(((RCC->CR>>25) & 0x1) != 1);  //�ȴ�PLL����
    RCC->CFGR |= 0x2;                   //PLL��ΪsysClk
    while(((RCC->CFGR>>2) & 0x3) != 0x02);  //�ȴ�sysClk����
#else
	#warning "Make sure needn't define STM32F051 etc. ?"
#endif
  }
  else
  {
    RCC->CFGR &= 0xFFFEFFFF; //HSI 8MHz/2
    RCC->CFGR |= ((clkMHz>>2)-2)<<18; //����PLLֵ��Ƶ
    
    //���ڵȴ�״̬ʱ�������ȿ���Ԥȡ������
    if (clkMHz <= 24)
      FLASH->ACR |= 0x30;//000����ȴ�״̬���� 0 < SYSCLK �� 24MHz
    else if (clkMHz <= 48)
      FLASH->ACR |= 0x31;//001��һ���ȴ�״̬���� 24MHz < SYSCLK �� 48MHz
    else
      FLASH->ACR |= 0x32;//010�������ȴ�״̬���� 48MHz < SYSCLK �� 72MHz
    
    RCC->CR |= 1<<24;                   //PLLON
    while(((RCC->CR>>25) & 0x1) != 1);  //�ȴ�PLL����
    RCC->CFGR |= 0x2;                   //PLL��ΪsysClk
    while(((RCC->CFGR>>2)&0x3) != 0x02);  //�ȴ�sysClk����
  }
}

/**
  * <b>оƬ�ϵ�Ĭ��ʱ�����á�</b>
  * <p>���ܣ������ָ�ʱ��ϵͳ���ϵ�Ĭ��״̬��
  * @version		1.1.2014.0228
  * <p>ʵ��CM0��CM3������ɣ�
  * @author             Dylan
  * @see                Javadocע��/Eclipse�༭
  */
void onPowerOnClock(void)
{
#if defined  (STM32F051) || defined  (STM32F042)
	RCC->CR |= 0x00000001;//HSION

	RCC->CR &= 0x0202FFFF;/* Reset HSEON, CSSON and PLLON, HSEBYP bit */

	RCC->CFGR &= 0x0000000C;/* Reset SW[1:0], HPRE[3:0], PPRE[2:0], ADCPRE and MCOSEL[2:0],PLLSRC, PLLXTPRE and PLLMUL[3:0] bits */

	RCC->CFGR2 &= 0x00000000;/* Reset PREDIV1[3:0] bits *///��CFGR[17]һ������HSE-PLL����

	RCC->CFGR3 &= 0x00000000;/* Reset USARTSW[1:0], I2CSW, CECSW and ADCSW bits */

	RCC->CR2 &= 0xFFFFFFFE;/* Reset HSI14 bit */

	RCC->CIR = 0x00000000;/* Disable all interrupts */

#elif defined (STM32F100) || defined (STM32F101) || defined (STM32F103)
	RCC->CR &= 0x0000FF83;//��HSI������8MHz��1%==������HSI==���ȴ�����

	RCC->CFGR = 0x00000400;//����APB1ʱ����HCLK_Div2

	RCC->CIR = 0x00BF0000;//������е�RCC�жϱ�־

	//����������
	#ifdef  VECT_TAB_RAM
	/* Set the Vector Table base location at 0x20000000 */
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
	#else  /* VECT_TAB_FLASH  */
	/* Set the Vector Table base location at 0x08000000 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
	#endif
#else
	#warning "Make sure needn't define STM32F051 etc. ?"
#endif
}

/**
  * <b>NVIC�жϿ�������</b>
  * <p>���ܣ��ж����ȼ���ʹ�ܡ�  
  * @param  NVIC_PriorityGroup  �жϷ���
  * @param  NVIC_IRQn           �жϴ������
  * @param  NVIC_Priority       �ж����ȼ�
  * @version            1.0.2014.0228
  * <p>��ֲ��
  * @author             Dylan
  * @see                Javadocע��/Eclipse�༭
  */
void NVIC_GroupSet(u32 NVIC_PriorityGroup, s16 NVIC_IRQn, u16 NVIC_Priority)
{
#if defined  (STM32F051) || defined  (STM32F042)
	//null
#elif defined (STM32F100) || defined (STM32F101) || defined (STM32F103)
	assert_param(IS_NVIC_PRIORITY_GROUP(NVIC_PriorityGroup));//��������Ч��

	SCB->AIRCR = (u32)0x05FA0000 | NVIC_PriorityGroup;//�ȼ���Կ��(u32)0x05FA0000,Ȼ�����÷���

	NVIC->ISER[((NVIC_IRQn)>>5)] = (1<<((NVIC_IRQn)&0x1F));//ʹ���ж�

	//ÿ��ֻ�ܴ�4�����õ����ַ(NVIC_IRQn>>2)
	//�����ڵ�ƫ��(NVIC_IRQn&0x3),�õ�ƫ�Ƶ�ȷ��λ��*8+4
	if(NVIC_IRQn < 0) /* set Priority for Cortex-M3 System Interrupts */
	{
	SCB->SHP[((NVIC_IRQn)&0xF)-4] = ((NVIC_Priority<<4)&0xFF);
	}
	else//���������ж����ȼ�,���ȼ���->��(0-0xFF)
	{
	NVIC->IP[(NVIC_IRQn)] = ((NVIC_Priority<<4)&0xFF);//������Ӧ���ȼ����������ȼ�,��ʽΪ0x?0(?=0~F)
	}
#else
	#warning "Make sure needn't define STM32F051 etc. ?"
#endif
}

/** <summary>
  * Configure all unused GPIO port pins in Analog Input mode,
  * this will reduce the power consumption and increase the device immunity against EMI/EMC.
  * </summary>
  * <param name=""></param>
  * <exception cref=""></exception>
  */
void SetUnusedGPIOsToEMCMode(void)
{
#if defined  (STM32F051)
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);

	//PA13��PA14(SWD)���ָ�λ״̬������ѡ��ģ��ģʽ��
	GPIOA->MODER   = 0x28FFFFFF;
	GPIOA->OTYPER  = 0x00000000;
	GPIOA->OSPEEDR = 0x00000000;
	GPIOA->PUPDR   = 0x24000000;

	GPIOB->MODER   = 0xFFFFFFFF;
	GPIOB->OTYPER  = 0x00000000;
	GPIOB->OSPEEDR = 0x00000000;
	GPIOB->PUPDR   = 0x00000000;

#elif defined (STM32F100) || defined (STM32F101) || defined (STM32F103)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_Init(GPIOB, &GPIO_InitStructure);

#elif defined  (STM32F042)

#else
	#warning "Make sure needn't define STM32F051 etc. ?"
#endif
}

/** <summary>
  * ��IWDG��
  * </summary>
  * <param name="pr">40KHz���Ź�ʱ�ӵ�Ԥ��Ƶֵ��</param>
  * <param name="rlr">���Ź������������ֵ��Max=0xFFF��</param>
  * <exception cref=""></exception>
  */
void OpenIWDG(u8 pr, u16 rlr)
{
  IWDG->KR = 0x5555;	//ʹ�ܶ�IWDG->PR��IWDG->RLR��д
  IWDG->PR = pr;		//LSI/32=40Khz/(4*2^pre) //4��8��16��32��64��128��256
  IWDG->RLR = rlr;		//�Ӽ��ؼĴ��� IWDG->RLR��Max=0xFFF����Ϊ���Ź����������Ǵ����ֵ��ʼ���¼�����
  IWDG->KR = 0xAAAA;	//reload
  IWDG->KR = 0xCCCC;	//ʹ�ܿ��Ź�
}

/** <summary>
  * ��IWDGι����
  * </summary>
  * <param name=""></param>
  * <exception cref=""></exception>
  */
void FeedIWDG(void)
{
  IWDG->KR = 0xAAAA;	//reload
}

/** <summary>
  * ����IWDG��
  * </summary>
  * <param name="resetTimeout">�������Ź���λ�ĳ�ʱʱ�䡣</param>
  * <exception cref=""></exception>
  */
void StartIWDG(u16 resetTimeout)
{
  /* Check IWDGRST flag if the system has resumed from IWDG reset */
  if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
  {
    RCC_ClearFlag();
    //������Դ򿪷��������б�����
  }
  else
  {
  }

  resetTimeout = (resetTimeout + (resetTimeout>>2)) & 0xFFF; //��Ϊʱ��ѡȡ����1.25kHz�����Լ���ֵ��1.25����

  if (resetTimeout < 100)
  {
	  resetTimeout = 125; //��֤����100ms
  }

  OpenIWDG(0x3, resetTimeout);	//40Khz/32=1.25kHz
}
