/* Includes ------------------------------------------------------------------*/
//mian.h�������������������Ҫ��ͷ�ļ�
#include "main.h"
#include  "FreeRTOS.h"
#include "task.h"
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
//����Ƽ���Ʒ
//�з��Ŷӣ����������Ŷ�
//��ϵ��ʽ��QQȺ��471023785
            ���䣺qitiansizhou@163.com
            �Ա���http://shop125061094.taobao.com/
//�������˳���ֻ��������ѧϰ��ѧ����ֹ������ҵ��;���������ؾ���
//�汾��V2.8
//���ڣ�20150727
//�޸�˵����
//
1���������ű�������
2�����뷢�ͼ�����
3, ��ֲ��FreeRtos(ʹ������ѭFreeRTOS��ؿ�ԴЭ�飬���������ڽ�ѧ)
*/
void My_System_Init(void);
//extern __IO uint16_t ADCConvertedValue;
extern uint16_t ADC_Result;
extern uint8_t NRF24L01_TXDATA[RX_PLOAD_WIDTH];
void NRF_Send( void *pvParameters1 );
void ADC_Check( void *pvParameters2 );
void Val_Check( void *pvParameters3 );
void LED1_blink( void *pvParameters4 );
void LED2_blink( void *pvParameters4 );
//void NRF_Send(void const * argument);
//void ADC_Check(void const * argument);
//void Val_Check(void const * argument);
void My_System_Init(void);
//extern __IO uint16_t ADCConvertedValue;
extern uint16_t ADC_Result;
extern uint8_t NRF24L01_TXDATA[RX_PLOAD_WIDTH];
void NRF_Send( void *pvParameters1 )
{
	while(1)
	{
		vTaskDelay(10);
		//����һ������ÿ�뷢50-100�����ݰ�
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6))
		{
			vTaskDelay(1);
		}
		NRF_IRQ();
		NRF_TxPacket(NRF24L01_TXDATA,32);
	}
}
void LED1_blink( void *pvParameters4 )
{
		while(1)
		{
			LED1_ON;
			vTaskDelay(100);
			LED1_OFF;
			vTaskDelay(100);
			//vTaskDelay(100);
		}
}
void LED2_blink( void *pvParameters4 )
{
		while(1)
		{
			LED2_ON;
			vTaskDelay(100);
			LED2_OFF;
			vTaskDelay(100);
			//vTaskDelay(100);
		}
}
void Val_Check( void *pvParameters2 )
{
		float Power_Check = 0.0;
		uint32_t RUN_Times = 0;
		while(1)
		{
		//������
		vTaskDelay(1000);
		//osDelay(500);
		adcInit5();
		Power_Check = ADC_Result*3.3*2/4096.0;
		if(Power_Check < 3.7)
		{
			//��������ʾ��������
			if(RUN_Times%12 == 0)
			{
				LEDALL_OFF;
			}
			else if(RUN_Times%22 == 0)
			{
				LEDALL_ON;
			}
			RUN_Times ++;
			if(RUN_Times > 1000000)
			{
				RUN_Times = 0;
			}
		}
		else
		{
			LEDALL_ON;
		}
	}
}
void ADC_Check( void *pvParameters3 )
{
	static uint32_t NRF_Counter = 0;
	static uint32_t NRF_Check_Sum = 0;
//	float Power_Check = 0.0;
	uint8_t i;
//	uint32_t RUN_Times = 0;
	while(1)
	{
		//����ɨ��ң�صĸ���ҡ�˵�λ�ã�Ȼ����NRF����ȥ
		//��ͷ�Լ����͵Ĵ���0-99ѭ������
		NRF24L01_TXDATA[0] = 0XA5;
		NRF_Counter ++;
		NRF24L01_TXDATA[1] = NRF_Counter;
		if(NRF_Counter >= 99)
			NRF_Counter = 0;
		
		//�������
		adcInit1();
		ADC_Result = ADC_Result/4;
		NRF24L01_TXDATA[2] = (uint8_t)(ADC_Result&0x00ff);
		NRF24L01_TXDATA[3] = (uint8_t)((ADC_Result>>8)&0x00ff);

		//���YAW
		adcInit0();
		ADC_Result = ADC_Result/4;
		NRF24L01_TXDATA[4] = (uint8_t)(ADC_Result&0x00ff);
		NRF24L01_TXDATA[5] = (uint8_t)((ADC_Result>>8)&0x00ff);
		
		//���Pitch
		adcInit4();
		ADC_Result = 4096 - ADC_Result;
		ADC_Result = ADC_Result/4;
		NRF24L01_TXDATA[6] = (uint8_t)(ADC_Result&0x00ff);
		NRF24L01_TXDATA[7] = (uint8_t)((ADC_Result>>8)&0x00ff);
		
		//���Roll
		adcInit6();
		ADC_Result = ADC_Result/4;
		NRF24L01_TXDATA[8] = (uint8_t)(ADC_Result&0x00ff);
		NRF24L01_TXDATA[9] = (uint8_t)((ADC_Result>>8)&0x00ff);
		
		//��ⰴ��
		if(KEY1_ON)
			NRF24L01_TXDATA[10] = 1;
		else if(KEY2_ON)
			NRF24L01_TXDATA[10] = 2;	
		else
			NRF24L01_TXDATA[10] = 0;
		
		//������P
		adcInit3();
		ADC_Result = ADC_Result/16;
		NRF24L01_TXDATA[20] = (uint8_t)(ADC_Result&0x00ff);
		
		//������D
		adcInit2();
		ADC_Result = ADC_Result/16;
		NRF24L01_TXDATA[21] = (uint8_t)(ADC_Result&0x00ff);
		
		//���к�У�鴦��
		NRF_Check_Sum = 0;
		for(i=0;i<30;i++)
		{
				NRF_Check_Sum = NRF_Check_Sum + NRF24L01_TXDATA[i];
		}
		NRF24L01_TXDATA[30] = (uint8_t)(NRF_Check_Sum&0x00ff);
		NRF24L01_TXDATA[31] = (uint8_t)((NRF_Check_Sum>>8)&0x00ff);
		//osDelay(20);
		vTaskDelay(10);
	}	
}
int main(void)
{
	BaseType_t TaskStatus = pdFALSE;
	My_System_Init(); //���ڸ�������ĳ�ʼ��
	//vTaskDelay(1000);
	if (SysTick_Config(SystemCoreClock / 1000))
	{
		/* Capture error */
		while (1);
	}

	/* Create tasks */
	TaskStatus = xTaskCreate(NRF_Send, (char const*)"NRF_Send_Task", 100, NULL, 5, NULL);
	TaskStatus = xTaskCreate(Val_Check, (char const*)"Val_Check_Task", 100, NULL, 4, NULL);
	TaskStatus = xTaskCreate(ADC_Check, (char const*)"ADC_Check_Task", 100, NULL, 3, NULL);

	if (TaskStatus == pdTRUE)
	{
		/* Start the FreeRTOS scheduler */
		//LEDALL_OFF;
		LEDALL_ON;
		delay_Ms_Loop(200);
		LEDALL_OFF;
		delay_Ms_Loop(200);
		LEDALL_ON;
		delay_Ms_Loop(200);
		LEDALL_OFF;
		delay_Ms_Loop(200);
		LEDALL_ON;
		vTaskStartScheduler();
	}
	else
	{
		while(1)
		{

			//vTaskDelay(1000);
		}
	}
	/* ���һ��������main()������Ӧ�û�ִ�е���������ִ�е�����ܿ������ڴ�ѿռ䲻�㵼�¿���
	�����޷��������������н�����������ڴ���������Ϣ */
	for(;;);
}
void NVIC_Configuration(void)
{
}

void My_System_Init(void)
{
	uint8_t flag;
	LED_INIT();
	Key_INIT();
	LEDALL_ON;
	//vTaskDelay(100);
	LED1_OFF;
	LED2_OFF;
	//while(1);
	//delay_Ms_Loop(1000);
	NVIC_Configuration();
	SPI1_INIT();
	flag = NRF_CHECK();//���NRFģ���Ƿ���������
	if(flag != 1)
	{
		while(1)
		{
			LED1_OFF;
			delay_Ms_Loop(200);
			LED1_ON;
			delay_Ms_Loop(200);
		}
	}
	NRF24L01_INIT();	//nRF24L01��ʼ��
//	NRF24L01_INIT();	//nRF24L01��ʼ��
	NRF_GPIO_Interrupt_Init();	//
	//��ֹ�������ͻȻ����
	while(1)
	{
				//�������
		adcInit1();
		ADC_Result = ADC_Result/4;
		if(ADC_Result < 100)
			break;
	}
	//LEDALL_ON;
	//vTaskDelay(100);
	LEDALL_ON;
}
void System_STate_Check(void)
{
		//float Battery_Vol = 0;
}
