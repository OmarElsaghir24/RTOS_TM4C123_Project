/*
 * FreeRTOSConfig.h
 *
 *  Created on: Sep 4, 2025
 *      Author: Omar Elsaghir
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <stdint.h>
#include "tm4c123gh6pm.h"

#define SCB_SHCSR_MEMFAULTENA_Msk  (1UL << 16)
#define SCB_SHCSR_BUSFAULTENA_Msk  (1UL << 17)
#define SCB_SHCSR_USGFAULTENA_Msk  (1UL << 18)


// Basic MCU settings
#define configCPU_CLOCK_HZ        ( ( unsigned long ) 80000000 )  // 80 MHz
#define configTICK_RATE_HZ        ( ( TickType_t ) 1000 )         // 1 ms tick
#define configMAX_PRIORITIES      ( 5 )
#define configMINIMAL_STACK_SIZE  ( 128 )
#define configTOTAL_HEAP_SIZE     ( ( size_t ) ( 25 * 1024 ) )
#define configMAX_TASK_NAME_LEN   ( 20 )
#define configCHECK_FOR_STACK_OVERFLOW 2
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1
#define configUSE_TASK_FPU_SUPPORT 1
#define INCLUDE_uxTaskGetStackHighWaterMark 1
#define configUSE_TRACE_FACILITY        1
#define configUSE_STATS_FORMATTING_FUNCTIONS 1
#define configGENERATE_RUN_TIME_STATS   1
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()   Timer1_Init()
#define portGET_RUN_TIME_COUNTER_VALUE()           (0xFFFFFFFF - TIMER1_TAV_R)


// Tick type size (pick ONE)
#define configUSE_16_BIT_TICKS    0   // Use 32-bit ticks

// Scheduler options
#define configUSE_PREEMPTION      1         // Switch to 0 for Cooperative mode
#define configUSE_IDLE_HOOK       1
#define configUSE_TICK_HOOK       0
#define configUSE_MUTEXES         1
#define configUSE_TIMERS          1
#define configTIMER_TASK_PRIORITY   ( configMAX_PRIORITIES - 1 )
#define configTIMER_QUEUE_LENGTH   5
#define configTIMER_TASK_STACK_DEPTH  256
//#define configUSE_TIME_SLICING    1
//#define configUSE_QUEUE_SETS      1
#define configUSE_HEAP_SCHEME 4   // just for clarity if you want
#define configUSE_HEAP4       1   // optional flag for your notes
#define configUSE_MALLOC_FAILED_HOOK 1

#define configUSE_TRACE_FACILITY 1
#define configUSE_STATS_FORMATTING_FUNCTIONS 1

#define configUSE_HEAP_CLZ_CHECK   0

#define configUSE_HEAP_SCHEME_EXTENSIONS 1

//SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk;  // MemManage fault enable


// Cortex-M specific interrupt settings
#define configPRIO_BITS                           3  // TM4C123 = 3 bits (0-7)
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY   7
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY  2
#define configKERNEL_INTERRUPT_PRIORITY           ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
#define configMAX_SYSCALL_INTERRUPT_PRIORITY      ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

// API functions
#define INCLUDE_vTaskDelay        1
#define INCLUDE_vTaskDelete       1
#define INCLUDE_vTaskSuspend      1
#define INCLUDE_vTaskPrioritySet  1

#endif /* FREERTOS_CONFIG_H */
