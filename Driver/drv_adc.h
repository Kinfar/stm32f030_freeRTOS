#ifndef _DRV_ADC_
#define _DRV_ADC_
#include "stm32f0xx.h"
#include "stm32f0xx_adc.h"
//typedef enum {
//    ADC_BATTERY = 0,
//    ADC_EXTERNAL_PAD = 1,
//    ADC_EXTERNAL_CURRENT = 2,
//    ADC_RSSI = 3,
//    ADC_CHANNEL_MAX = 1
//} AdcChannel;

//typedef struct drv_adc_config_t {
//    uint8_t powerAdcChannel;     // which channel used for current monitor, allowed PA1, PB1 (ADC_Channel_1, ADC_Channel_9)
//    uint8_t rssiAdcChannel;      // which channel used for analog-rssi (RC-filter), allowed PA1, PB1 (ADC_Channel_1, ADC_Channel_9)
//} drv_adc_config_t;

void adcInit0(void);
void adcInit1(void);
void adcInit4(void);
void adcInit6(void);
void adcInit3(void);
void adcInit2(void);
void adcInit5(void);
//uint16_t adcGetadcGetValue(void);
#endif


