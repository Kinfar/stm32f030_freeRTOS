/**
  ******************************************************************************
  * @file /Hardware.c
  * @author  xd.wu
  * @brief  硬件接口配置
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
  * 初始化普通的输入/输出引脚。
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
	
	// GPIO的公共特性
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	//输入引脚 KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = SPLIT_DEFINE_PIN(ST_KEY_ONOFF);
	GPIO_Init(SPLIT_DEFINE_PORT(ST_KEY_ONOFF), &GPIO_InitStructure);
	
	//输入引脚 KEY2
	GPIO_InitStructure.GPIO_Pin = SPLIT_DEFINE_PIN(ST_KEY_SET);
	GPIO_Init(SPLIT_DEFINE_PORT(ST_KEY_SET), &GPIO_InitStructure);
	
	//输入引脚 KEY3
	GPIO_InitStructure.GPIO_Pin = SPLIT_DEFINE_PIN(ST_KEY3);
	GPIO_Init(SPLIT_DEFINE_PORT(ST_KEY3), &GPIO_InitStructure);
	
	//输入引脚 KEY4
	GPIO_InitStructure.GPIO_Pin = SPLIT_DEFINE_PIN(ST_KEY_UP);
	GPIO_Init(SPLIT_DEFINE_PORT(ST_KEY_UP), &GPIO_InitStructure);
	
	//输入引脚 KEY5
	GPIO_InitStructure.GPIO_Pin = SPLIT_DEFINE_PIN(ST_KEY_DOWN);
	GPIO_Init(SPLIT_DEFINE_PORT(ST_KEY_DOWN), &GPIO_InitStructure);
	
#elif  defined (STM32F042)
	//GPIOA
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
	//输入引脚 RUN_LED
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
  * <b>配置串行外设接口。</b>
  * <p>功能：初始化SPI1配置。
  * <p>注意：STM32F0x系列的接收中断需要通过SPI_RxFIFOThresholdConfig()来指定触发中断的数据量！！
  * @param  BaudRatePCLKDiv     通信速率相关的预分频。SPI_BaudRatePrescaler_16 与APB2时钟相关
  * @param  CPOL                时钟极性，决定空闲时，SCK的电平状态。SPI_CPOL_Low
  * @param  CPHA_Edge           时钟相位，决定数据采样的边沿，上升还是下降沿。SPI_CPHA_1Edge
  * @version		        v1.0(2013-12-5)
  * <p>重写函数的功能说明！
  * @author 		        Dylan
  * @see                        Javadoc注释/Eclipse编辑
  */
void connectSPI1ToExt(u16 BaudRatePCLKDiv, u16 CPOL, u16 CPHA_Edge)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

#if  defined (STM32F042)
	//GPIOA
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  //SPIx_SCK 主模式 推挽复用输出
  //SPIx_MOSI 全双工模式/主模式 推挽复用输出
  //SPIx_MISO 全双工模式/主模式 浮空输入或带上拉输入
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
	
  //输入
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
  
  //1: 如果FIFO 的存储水平大于或等于1/4（8 位），产生RXNE 事件。
  SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF);
  
  SPI_Cmd(SPI1, ENABLE);
}


/**
  * <b>配置通用异步收发器。</b>
  * <p>功能：初始化UART2配置。
  * @param  baudrate    波特率
  * @param  parity      校验位。0 None,1 Odd, 2 Even
  * @param  length      数据位个数。带校验7 or 无校验8
  * @param  stopbits    停止位个数。1 1bit, 2 2bits, 3 1.5bits
  * @version            v1.0(2014-9-1)
  * <p>重写函数的功能说明！
  * @author             Dylan
  * @see                Javadoc注释/Eclipse编辑
  */
void connectUsart2ToExt(u32 baudrate, u8 parity, u8 length, u8 stopbits)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;  
	
#if  defined (STM32F042)
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
	
#else
	#warning "Make sure needn't define STM32F051 etc. ?"
#endif

  USART_Cmd(USART2, DISABLE); //用作重新配置USART2，先关串口

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, DISABLE);
  
  USART_InitStructure.USART_BaudRate = baudrate;
  
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
  
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure);
  
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USART2, USART_IT_TXE, DISABLE);

  USART_Cmd(USART2, ENABLE);
}

/**
  * <b>配置通用定时器。</b>
  * <p>功能：初始化TIM2-CH2配置。
  * @param  TIMx                定时器x
  * @param  SystemCLK           系统的主时钟
  * @param  LoadingValue        加载值，就是触发中断的时间间隔。
  * @version		1.1.2014.0228
  * <p>实现CM0与CM3兼容完成！
  * @author                     Dylan
  * @see                        Javadoc注释/Eclipse编辑
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
	SetAF_TIM2_CH1;//必须选择复用功能

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2, DISABLE);

	// TIMx寄存器配置
	TIMx->PSC = SystemCLK -1;
	TIMx->ARR = LoadingValue-1;

	TIMx->CR1 = 0x0000;

	TIMx->CCMR1 &= 0xFF00;//CH1冻结模式,无输出
	//TIMx->CCMR1 |= 0x0000;

	TIMx->CCER &= 0xFFF0;
	//TIMx->CCER |= 0x0000;

	TIMx->CCR1 = TIMx->ARR;//计数到达最大值ARR

	TIMx->DIER |= 0x0002;//Timx ccr1 iqr enable

	TIMx->CR1 |= 0x1;//timx enable
#else
	#warning "Make sure needn't define STM32F051 etc. ?"
#endif
}

/**
  * <b>系统滴答器的循环精确延时函数。</b>
  * <p>功能：精确的微秒延时，每隔一微秒计数减一。
  * @param  nus         需要延时的微秒数
  * @param  HCLK        系统的主时钟
  * @version            v1.0(2013-12-5)
  * <p>重写函数的功能说明！
  * @author             Dylan
  * @see                Javadoc注释/Eclipse编辑
  */
void Delay_us(u32 nus, u8 HCLK)
{  
  SysTick->CTRL &= 0x00010006;

  //(nus*HCLK/8)&0x00FFFFFF; //先对整体放大8倍，计算计数值并截取有效范围，再将结果缩小还原并赋值。
  SysTick->LOAD = ((nus*HCLK)&0x07FFFFF8)>>3; //Max=(2^24)/HCLK=16.7s/HCLK

  SysTick->VAL = 0x00;  //清空计数器当前值
  
  SysTick->CTRL |= 0x00001; //使能计数器
  
  while((SysTick->CTRL & 0x10001) == 0x1);//计数器处于使能状态下，直到将CountFlag=1
  
  SysTick->CTRL &= 0x00010006;  //复位,关闭计数器
}

/**
  word 字,byte 字节,bit 位
  一般取末尾8页地址，每页1KB
  地址计算 = (烧录程序的KB+1)*0x0400 +(0x0800 0000)
  而且地址在容量范围内
  闪存存储器容量
  4 = 16K字节的闪存存储器
  6 = 32K字节的闪存存储器
  8 = 64K字节的闪存存储器
  B = 128K字节的闪存存储器
  C = 256K字节的闪存存储器
  D = 384K字节的闪存存储器
  E = 512K字节的闪存存储器
  //例如，32K地址范围0x0800 0000~0x0800 7FFF
  大容量产品，写保护是以每2页为单位实现的，剩下的存储器块(第62页至第255页)则是同时提供保护。
  小容量和中容量的产品，写保护是以每4页为单位实现的。
*/
/*-------  下面的宏 FlashSizeKB、ProgramSizeKB ***很重要，当用到Flash时需要校对 -------*/
  #define FlashSizeKB     Current_FlashSizeKB//64  //Flash容量
  #define ProgramSizeKB   Current_ProgramSizeKB//14  //程序容量
  #define FlashStartAddr  ((u32)(0x08000000 + (ProgramSizeKB<<10)))  //程序下一页起始地址
  #define FlashEndAddr    ((u32)(0x08000000 + (FlashSizeKB<<10)-1))  //Flash结束地址
  #define FlashWRPR_Value ((u32)(0xFFFFFFFF << (u8)((ProgramSizeKB>>2) + 1)))//4页

/**
  * <b>数据保存到Flash的函数。</b>
  * <p>功能：在Flash地址页中写入单字，16位。
  * @param  addr        待写入的目的地址
  * @param  data        待写入的数据
  * @return             是否成功写入操作。
  * @version		1.1.2014.0228
  * <p>实现CM0与CM3兼容完成！
  * @author             Dylan
  * @see                Javadoc注释/Eclipse编辑
  */
bool Flash_wWord(u32 addr, u16 data)
{
	u32 Protected;
	FLASH_Status  FLASHStatus;
	
  //地址有效否
  if ((addr >= FlashStartAddr) && (addr <= FlashEndAddr))
  {
    //CR_LOCK位=1执行解锁序列
    FLASH->KEYR = 0x45670123; 
    FLASH->KEYR = 0xCDEF89AB;
    
    // Clear All flags
    FLASH->SR = 0x35;
    
    //地址所在页到Flash页结束，是否存在写保护
    Protected = (FLASH->WRPR) & FlashWRPR_Value;
  
    if (Protected == 0x00)  //写保护
    {
#if defined  (STM32F051) || defined  (STM32F042)
      FLASH_OB_Erase();//Erases the FLASH option bytes.
#elif defined (STM32F100) || defined (STM32F101) || defined (STM32F103)
      FLASH_EraseOptionBytes();//Erases the FLASH option bytes.
#else
	#warning "Make sure needn't define STM32F051 etc. ?"
#endif
      /* ---------------------------------------------------------------------------------------
      要将访问钥匙 0x05FA0000 与我们的操作 SYSRESETREQ[2]=1 相或,一起写入 SCB->AIRCR,才被CM3接受
      //NVIC_GenerateSystemReset();
      --------------------------------------------------------------------------------------- */
      SCB->AIRCR = (u32)0x05FA0000 | (u32)0x04;//先键入钥匙(u32)0x05FA0000,然后产生一个系统复位
    }
    else
    {
      // Clear All flags
      FLASH->SR = 0x35;
  
      //擦除指定页 FLASH pages
      FLASHStatus = FLASH_ErasePage(addr);  //修改闪存地址寄存器FLASH_AR，来选择要擦除的页
      
      if (FLASHStatus == FLASH_COMPLETE)
        FLASHStatus = FLASH_ProgramHalfWord(addr, data);

      //读取写入的数据，比对
      if((*(vu16*) addr) == data)
        return TRUE;
    }
  }
  return FALSE;
}

/**
  * <b>数组保存到Flash的函数。</b>
  * <p>功能：在Flash地址页中写入单字，16位。
  * @param  addr        待写入的目的地址
  * @param  pData       待写入的数组指针
  * @param  len         待写入的数据长度
  * @return             是否成功写入操作。
  * @version		1.1.2014.0228
  * <p>实现CM0与CM3兼容完成！
  * @author             Dylan
  * @see                Javadoc注释/Eclipse编辑
  */
bool Flash_wWords(u32 addr, u16 *pData, u16 len)
{
	u32 ProtectedPages;
	FLASH_Status FLASHStatus;
	u16 i;
	
  //地址有效否
  if ((addr >= FlashStartAddr) && ((addr+(len<<1)) <= FlashEndAddr))
  {
    //CR_LOCK位=1执行解锁序列
    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xCDEF89AB;
    
    // Clear All flags
    FLASH->SR = 0x35;
    
    //地址所在页到Flash页结束，是否存在写保护
    ProtectedPages = (FLASH->WRPR) & FlashWRPR_Value;
  
    if (ProtectedPages == 0x00) //写保护
    {
#if defined  (STM32F051) || defined  (STM32F042)
      FLASH_OB_Erase();//Erases the FLASH option bytes.
#elif defined (STM32F100) || defined (STM32F101) || defined (STM32F103)
      FLASH_EraseOptionBytes();//Erases the FLASH option bytes.
#else
	#warning "Make sure needn't define STM32F051 etc. ?"
#endif

      SCB->AIRCR = (u32)0x05FA0000 | (u32)0x04;//先键入钥匙(u32)0x05FA0000,然后产生一个系统复位
    }
    else
    {
      // Clear All flags
      FLASH->SR = 0x35;
  
      //擦除指定页 FLASH pages
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
  * <b>配置系统主时钟。</b>
  * <p>功能：选择系统时钟源和系统时钟频率。
  * @param  oscSelect   时钟源选择？外部8M高速晶振HSE ：内部8M低速晶振HSI
  * @param  clkMHz      芯片最大主频？M，查询对应的数据手册
  * @version		1.1.2014.0228
  * <p>实现CM0与CM3兼容完成！
  * @author             Dylan
  * @see                Javadoc注释/Eclipse编辑
  */
void createMasterClock(uint8_t oscSource, u8 clkMHz)
{
  onPowerOnClock();
  
  if (oscSource == USE_EXTERNAL_HSE)
  {
#if defined  (STM32F051) || defined  (STM32F042)
    RCC->CR |= 1<<16;       //HSEON
    while(((RCC->CR>>17) & 0x1) != 1);  //等待HSE就绪
    
    RCC->CFGR |= (clkMHz/8-2)<<18; //设置PLL值倍频//HSE 8MHz
    
    RCC->CFGR |= 1<<16; //切换PLL输入时钟源为0->1：HSI/2(4MHz)->HSE(8MHz)
    
    //存在等待状态时，必须先开启预取缓冲区
    if (clkMHz <= 24)
      FLASH->ACR |= 0x30;//000：零等待状态，当 0 < SYSCLK ≤ 24MHz
    else if (clkMHz <= 48)
      FLASH->ACR |= 0x31;//001：一个等待状态，当 24MHz < SYSCLK ≤ 48MHz
    else
      FLASH->ACR |= 0x32;//010：两个等待状态，当 48MHz < SYSCLK ≤ 72MHz
    
    RCC->CR |= 1<<24;                   //PLLON
    while(((RCC->CR>>25) & 0x1) != 1);  //等待PLL就绪
    RCC->CFGR |= 0x2;                   //PLL作为sysClk
    while(((RCC->CFGR>>2) & 0x3) != 0x02);  //等待sysClk就绪
#elif defined (STM32F100) || defined (STM32F101) || defined (STM32F103)
    RCC->CR |= 1<<16;       //HSEON
    while(((RCC->CR>>17) & 0x1) != 1);  //等待HSE就绪
    
    RCC->CFGR |= (clkMHz/8-2)<<18; //设置PLL值倍频//HSE 8MHz
    
    RCC->CFGR |= 1<<16; //切换PLL输入时钟源为0->1：HSI/2(4MHz)->HSE(8MHz)
    
    //存在等待状态时，必须先开启预取缓冲区
    if (clkMHz <= 24)
      FLASH->ACR |= 0x30;//000：零等待状态，当 0 < SYSCLK ≤ 24MHz
    else if (clkMHz <= 48)
      FLASH->ACR |= 0x31;//001：一个等待状态，当 24MHz < SYSCLK ≤ 48MHz
    else
      FLASH->ACR |= 0x32;//010：两个等待状态，当 48MHz < SYSCLK ≤ 72MHz
    
    RCC->CR |= 1<<24;                   //PLLON
    while(((RCC->CR>>25) & 0x1) != 1);  //等待PLL就绪
    RCC->CFGR |= 0x2;                   //PLL作为sysClk
    while(((RCC->CFGR>>2) & 0x3) != 0x02);  //等待sysClk就绪
#else
	#warning "Make sure needn't define STM32F051 etc. ?"
#endif
  }
  else
  {
    RCC->CFGR &= 0xFFFEFFFF; //HSI 8MHz/2
    RCC->CFGR |= ((clkMHz>>2)-2)<<18; //设置PLL值倍频
    
    //存在等待状态时，必须先开启预取缓冲区
    if (clkMHz <= 24)
      FLASH->ACR |= 0x30;//000：零等待状态，当 0 < SYSCLK ≤ 24MHz
    else if (clkMHz <= 48)
      FLASH->ACR |= 0x31;//001：一个等待状态，当 24MHz < SYSCLK ≤ 48MHz
    else
      FLASH->ACR |= 0x32;//010：两个等待状态，当 48MHz < SYSCLK ≤ 72MHz
    
    RCC->CR |= 1<<24;                   //PLLON
    while(((RCC->CR>>25) & 0x1) != 1);  //等待PLL就绪
    RCC->CFGR |= 0x2;                   //PLL作为sysClk
    while(((RCC->CFGR>>2)&0x3) != 0x02);  //等待sysClk就绪
  }
}

/**
  * <b>芯片上电默认时钟配置。</b>
  * <p>功能：用来恢复时钟系统到上电默认状态。
  * @version		1.1.2014.0228
  * <p>实现CM0与CM3兼容完成！
  * @author             Dylan
  * @see                Javadoc注释/Eclipse编辑
  */
void onPowerOnClock(void)
{
#if defined  (STM32F051) || defined  (STM32F042)
	RCC->CR |= 0x00000001;//HSION

	RCC->CR &= 0x0202FFFF;/* Reset HSEON, CSSON and PLLON, HSEBYP bit */

	RCC->CFGR &= 0x0000000C;/* Reset SW[1:0], HPRE[3:0], PPRE[2:0], ADCPRE and MCOSEL[2:0],PLLSRC, PLLXTPRE and PLLMUL[3:0] bits */

	RCC->CFGR2 &= 0x00000000;/* Reset PREDIV1[3:0] bits *///与CFGR[17]一起用作HSE-PLL设置

	RCC->CFGR3 &= 0x00000000;/* Reset USARTSW[1:0], I2CSW, CECSW and ADCSW bits */

	RCC->CR2 &= 0xFFFFFFFE;/* Reset HSI14 bit */

	RCC->CIR = 0x00000000;/* Disable all interrupts */

#elif defined (STM32F100) || defined (STM32F101) || defined (STM32F103)
	RCC->CR &= 0x0000FF83;//把HSI调整到8MHz±1%==》开启HSI==》等待就绪

	RCC->CFGR = 0x00000400;//限制APB1时钟是HCLK_Div2

	RCC->CIR = 0x00BF0000;//清除所有的RCC中断标志

	//配置向量表
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
  * <b>NVIC中断控制器。</b>
  * <p>功能：中断优先级、使能。  
  * @param  NVIC_PriorityGroup  中断分组
  * @param  NVIC_IRQn           中断处理程序
  * @param  NVIC_Priority       中断优先级
  * @version            1.0.2014.0228
  * <p>移植！
  * @author             Dylan
  * @see                Javadoc注释/Eclipse编辑
  */
void NVIC_GroupSet(u32 NVIC_PriorityGroup, s16 NVIC_IRQn, u16 NVIC_Priority)
{
#if defined  (STM32F051) || defined  (STM32F042)
	//null
#elif defined (STM32F100) || defined (STM32F101) || defined (STM32F103)
	assert_param(IS_NVIC_PRIORITY_GROUP(NVIC_PriorityGroup));//检测分组有效性

	SCB->AIRCR = (u32)0x05FA0000 | NVIC_PriorityGroup;//先键入钥匙(u32)0x05FA0000,然后设置分组

	NVIC->ISER[((NVIC_IRQn)>>5)] = (1<<((NVIC_IRQn)&0x1F));//使能中断

	//每组只能存4个，得到组地址(NVIC_IRQn>>2)
	//在组内的偏移(NVIC_IRQn&0x3),得到偏移的确切位置*8+4
	if(NVIC_IRQn < 0) /* set Priority for Cortex-M3 System Interrupts */
	{
	SCB->SHP[((NVIC_IRQn)&0xF)-4] = ((NVIC_Priority<<4)&0xFF);
	}
	else//配置外设中断优先级,优先级高->低(0-0xFF)
	{
	NVIC->IP[(NVIC_IRQn)] = ((NVIC_Priority<<4)&0xFF);//设置响应优先级和抢断优先级,格式为0x?0(?=0~F)
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

	//PA13、PA14(SWD)保持复位状态，其他选择模拟模式。
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
  * 打开IWDG。
  * </summary>
  * <param name="pr">40KHz看门狗时钟的预分频值。</param>
  * <param name="rlr">看门狗倒计数的最大值，Max=0xFFF。</param>
  * <exception cref=""></exception>
  */
void OpenIWDG(u8 pr, u16 rlr)
{
  IWDG->KR = 0x5555;	//使能对IWDG->PR和IWDG->RLR的写
  IWDG->PR = pr;		//LSI/32=40Khz/(4*2^pre) //4、8、16、32、64、128、256
  IWDG->RLR = rlr;		//从加载寄存器 IWDG->RLR，Max=0xFFF。因为看门狗计数器正是从这个值开始向下计数。
  IWDG->KR = 0xAAAA;	//reload
  IWDG->KR = 0xCCCC;	//使能看门狗
}

/** <summary>
  * 给IWDG喂狗。
  * </summary>
  * <param name=""></param>
  * <exception cref=""></exception>
  */
void FeedIWDG(void)
{
  IWDG->KR = 0xAAAA;	//reload
}

/** <summary>
  * 启动IWDG。
  * </summary>
  * <param name="resetTimeout">触发看门狗复位的超时时间。</param>
  * <exception cref=""></exception>
  */
void StartIWDG(u16 resetTimeout)
{
  /* Check IWDGRST flag if the system has resumed from IWDG reset */
  if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
  {
    RCC_ClearFlag();
    //例如可以打开蜂鸣器进行报警。
  }
  else
  {
  }

  resetTimeout = (resetTimeout + (resetTimeout>>2)) & 0xFFF; //因为时钟选取的是1.25kHz，所以计数值是1.25倍。

  if (resetTimeout < 100)
  {
	  resetTimeout = 125; //保证最少100ms
  }

  OpenIWDG(0x3, resetTimeout);	//40Khz/32=1.25kHz
}
