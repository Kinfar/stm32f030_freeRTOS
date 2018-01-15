
#ifndef _SYSCONFIG_H
#define _SYSCONFIG_H

#include "stdint.h"
#include "stm32f0xx.h"



typedef struct{
	int16_t ROLL;
	int16_t PITCH;
	int16_t THROTTLE;
	int16_t YAW;
}T_RC_Data;

typedef struct{	
	volatile float w_roll;
	volatile float w_pitch;
	volatile float w_yaw;
	volatile float angle_roll;
	volatile float angle_pitch;
	volatile float angle_yaw;
	volatile float T;
}W_AND_ANGLE;



extern W_AND_ANGLE  w_and_angle;		//这两个都是要在其他函数中使用的变量，所以放在此处
extern T_RC_Data 	Rc_Data;			//1000~2000	

#endif
	
