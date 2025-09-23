/*
 * user_created_tasks.h
 *
 *  Created on: Sep 12, 2025
 *      Author: Omar Elsaghir
 */

#ifndef USER_CREATED_TASKS_H_
#define USER_CREATED_TASKS_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

// Defines
#define PB1  PORTC, 4
#define PB2  PORTC, 5
#define PB3  PORTC, 6
#define PB4  PORTC, 7
#define PB5  PORTD, 6
#define PB6  PORTB, 3
#define GREEN_LED  PORTE,0
#define YELLOW_LED PORTA,4
#define WHITE_LED  PORTA,3
#define RED_LED    PORTA,2
#define RED_LED_BOARD  PORTF, 1
#define GREEN_LED_BOARD PORTF, 3

#define mainAUTO_RELOAD_TIMER_PERIOD pdMS_TO_TICKS( 500 )

#define LOG_QUEUE_LEN   16   // number of messages in queue
#define LOG_MSG_SIZE    128  // max length of each message

// Variables
SemaphoreHandle_t xUartMutex;
QueueHandle_t xLogQueue;

// Subroutines
uint8_t readPbs(void);
void vBlinkYellowTask(void *pvParameters);
void vBlinkRedTask(void *pvParameters);
void vBlinkGreenTask(void *pvParameters);
void vControllerTask(void *pvParameters);
void vMemoryTask(void *pvParameters);
void vShellTask(void *pvParameters);
void prvAutoReloadTimerCallback( TimerHandle_t xTimer );
void vMemTestTask(void *pvParameters);
void vStackMonitorTask(void *pvParameters);
void vUartLoggerTask(void *pvParameters);

#endif /* USER_CREATED_TASKS_H_ */
