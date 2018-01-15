#ifndef _SPI_H_
#define _SPI_H_
#include "stm32f0xx.h"
/*
6.2.4G:	CE:PB0
		CSN:PA4
		SCK:PA5
		MOSI:PA7
		MISO:PA6
		IRQ:PB1
*/
#define SPI_CE_H()   GPIO_SetBits(GPIOA, GPIO_Pin_8) 
#define SPI_CE_L()   GPIO_ResetBits(GPIOA, GPIO_Pin_8)

#define SPI_CSN_H()  GPIO_SetBits(GPIOA, GPIO_Pin_11)
#define SPI_CSN_L()  GPIO_ResetBits(GPIOA, GPIO_Pin_11)

void SPI1_INIT(void);
u8 SPI_RW(u8 dat);

#endif

