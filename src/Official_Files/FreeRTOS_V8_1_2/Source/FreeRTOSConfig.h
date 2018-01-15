/*
    FreeRTOS V8.1.2 - Copyright (C) 2014 Real Time Engineers Ltd. 
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that has become a de facto standard.             *
     *                                                                       *
     *    Help yourself get started quickly and support the FreeRTOS         *
     *    project by purchasing a FreeRTOS tutorial book, reference          *
     *    manual, or both from: http://www.FreeRTOS.org/Documentation        *
     *                                                                       *
     *    Thank you!                                                         *
     *                                                                       *
    ***************************************************************************

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.

    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available from the following
    link: http://www.freertos.org/a00114.html

    1 tab == 4 spaces!

    ***************************************************************************
     *                                                                       *
     *    Having a problem?  Start by reading the FAQ "My application does   *
     *    not run, what could be wrong?"                                     *
     *                                                                       *
     *    http://www.FreeRTOS.org/FAQHelp.html                               *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org - Documentation, books, training, latest versions,
    license and Real Time Engineers Ltd. contact details.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.OpenRTOS.com - Real Time Engineers ltd license FreeRTOS to High
    Integrity Systems to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

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
//版本：V2.8
//日期：20150727
//修改说明：
//
1，加入油门保护机制
2，加入发送检测机制
*/
#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/

#define configUSE_PREEMPTION						1			/* 使能抢占优先级 */
#define configUSE_IDLE_HOOK							0			/* 使能空闲钩子函数 */
#define configUSE_TICK_HOOK							0			/* 使能滴答器钩子函数 */

#define configCPU_CLOCK_HZ							( ( uint32_t ) 48000000 )

#define configTICK_RATE_HZ							( ( TickType_t ) 1000 )				/* 设置滴答器时钟频率 */
#define configMAX_PRIORITIES						( ( unsigned portBASE_TYPE ) 10 )	/* 配置最大优先级 */
#define configMINIMAL_STACK_SIZE					( ( unsigned short ) 60 )			/* 配置最小堆栈大小 */
#define configTOTAL_HEAP_SIZE						( ( size_t ) ( 3400 ) )			/* 设置堆的总容量, 计算入Keil的ZI. */

#define configMAX_TASK_NAME_LEN						( 16 )
#define configUSE_TRACE_FACILITY					1
#define configUSE_16_BIT_TICKS						0
#define configIDLE_SHOULD_YIELD						1
#define configUSE_MUTEXES               			1
#define configQUEUE_REGISTRY_SIZE					8
#define configCHECK_FOR_STACK_OVERFLOW				0
#define configUSE_RECURSIVE_MUTEXES					1
#define configUSE_MALLOC_FAILED_HOOK				0
#define configUSE_APPLICATION_TASK_TAG				0
#define configUSE_COUNTING_SEMAPHORES				1
#define configGENERATE_RUN_TIME_STATS				0

/* Co-routine definitions. 联合程序 */
#define configUSE_CO_ROUTINES 						0
#define configMAX_CO_ROUTINE_PRIORITIES 			2

/* Software timer definitions. */
#define configUSE_TIMERS							1
#define configTIMER_TASK_PRIORITY					( 2 )
#define configTIMER_QUEUE_LENGTH					5
#define configTIMER_TASK_STACK_DEPTH				( 80 )

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet					1
#define INCLUDE_uxTaskPriorityGet					1
#define INCLUDE_vTaskDelete							1
#define INCLUDE_vTaskCleanUpResources				1
#define INCLUDE_vTaskSuspend						1
#define INCLUDE_vTaskDelayUntil						1
#define INCLUDE_vTaskDelay							1
	
/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
#define configASSERT( x ) 							if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }

/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names - or at least those used in the unmodified vector table. */
//#define vPortSVCHandler SVC_Handler
//#define xPortPendSVHandler PendSV_Handler
//#define xPortSysTickHandler SysTick_Handler


#endif /* FREERTOS_CONFIG_H */

