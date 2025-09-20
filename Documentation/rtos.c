

/**
 * rtos.c
 */
#include <inttypes.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "tm4c123gh6pm.h"
#include "gpio.h"
#include "uart0.h"
#include "wait.h"
#include "user_created_tasks.h"
#include "faults.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

#define CPACR_ADDR   (0xE000ED88UL)
#define CPACR        (*((volatile uint32_t *)CPACR_ADDR))

static inline void enableFPU(void)
{
    // Give full access to CP10 and CP11 (bits 20-23 = 0b1111)
    CPACR |= (0xF << 20);

    // Ensure subsequent instructions see the effect immediately
    __asm volatile ("dsb");
    __asm volatile ("isb");
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)xTask;
    (void)pcTaskName;
    // Blink LED here to signal overflow
    while (1)
        {
            setPinValue(GREEN_LED_BOARD, 1);
            vTaskDelay(pdMS_TO_TICKS(100));
            setPinValue(GREEN_LED_BOARD, 0);
            vTaskDelay(pdMS_TO_TICKS(100));
        }
}

void Timer1_Init(void)
{
    SYSCTL_RCGCTIMER_R |= 0x02;        // Enable Timer1 clock
    while ((SYSCTL_PRTIMER_R & 0x02) == 0) {} // wait until ready

    TIMER1_CTL_R = 0;                  // disable timer
    TIMER1_CFG_R = 0x00000000;         // 32-bit timer
    TIMER1_TAMR_R = 0x02;              // periodic timer mode
    TIMER1_TAILR_R = 0xFFFFFFFF;       // max value
    TIMER1_TAPR_R = 0;                 // no prescaler
    TIMER1_ICR_R = 0x1;                // clear timeout flag
    TIMER1_CTL_R = 0x01;               // enable timer
}

void initHw(void)
{
    // Enable clocks
    enablePort(PORTA);
    enablePort(PORTB);
    enablePort(PORTC);
    enablePort(PORTD);
    enablePort(PORTE);
    enablePort(PORTF);
    _delay_cycles(3);

    // Configure Leds as outputs
    selectPinPushPullOutput(RED_LED);
    selectPinPushPullOutput(GREEN_LED);
    selectPinPushPullOutput(YELLOW_LED);
    selectPinPushPullOutput(WHITE_LED);
    selectPinPushPullOutput(RED_LED_BOARD);
    selectPinPushPullOutput(GREEN_LED_BOARD);

    // Configure pushbuttons as inputs
    selectPinDigitalInput(PB1);
    enablePinPullup(PB1);

    selectPinDigitalInput(PB2);
    enablePinPullup(PB2);

    selectPinDigitalInput(PB3);
    enablePinPullup(PB3);

    selectPinDigitalInput(PB4);
    enablePinPullup(PB4);

    selectPinDigitalInput(PB5);
    enablePinPullup(PB5);

    selectPinDigitalInput(PB6);
    enablePinPullup(PB6);
}

int main(void)
{
    // Enable FPU
    enableFPU();

    initHw();
    UART0_Init(115200, 16e6);

    TimerHandle_t xAutoReloadTimer;
    BaseType_t xTimer1Started;

    // Create mutex
    xUartMutex = xSemaphoreCreateMutex();

    // Create queue
    xLogQueue = xQueueCreate(LOG_QUEUE_LEN, LOG_MSG_SIZE);

    // Create tasks
    xTaskCreate(vControllerTask, "Buttons", 256, NULL, 3, NULL);
    xTaskCreate(vShellTask, "Shell", 1024, NULL, 5, NULL);
    xTaskCreate(vMemoryTask, "Memory", 512, NULL, 2, NULL);
    xTaskCreate(vUartLoggerTask, "UART Logger", 512, NULL, 1, NULL);
    xTaskCreate(vStackMonitorTask, "Stack Monitor", 512, NULL, 3, NULL);

    // Create timer
    xAutoReloadTimer = xTimerCreate("Heart beat", mainAUTO_RELOAD_TIMER_PERIOD, pdTRUE, 0, prvAutoReloadTimerCallback );

    if( ( xAutoReloadTimer != NULL ) )
    {

           xTimer1Started = xTimerStart( xAutoReloadTimer, 0 );
           if( ( xTimer1Started == pdPASS ) )
           {

                 vTaskStartScheduler();
           }
    }

    //vTaskStartScheduler();
}
