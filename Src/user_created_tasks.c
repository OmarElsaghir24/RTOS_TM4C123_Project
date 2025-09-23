/*
 * user_created_tasks.c
 *
 *  Created on: Sep 12, 2025
 *      Author: Omar Elsaghir
 */
#include <inttypes.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "user_created_tasks.h"
#include "gpio.h"
#include "uart0.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
//#include "semphr.h"
//#include "timers.h"

// Variables
TaskHandle_t xLed1Handle = NULL;
TaskHandle_t xLed2Handle = NULL;
TaskHandle_t xLed3Handle = NULL;
char string[100];

// Subroutines
uint8_t readPbs(void)
{
    uint8_t num = 0;

        if(!getPinValue(PB1))
            num = 1;

        else if(!getPinValue(PB2))
            num = 2;

        else if(!getPinValue(PB3))
            num = 4;

        else if(!getPinValue(PB4))
            num = 8;

        else if(!getPinValue(PB5))
            num = 16;

        else if(!getPinValue(PB6))
            num = 32;

        return num;
}

void vUartLoggerTask(void *pvParameters)
{
    char msg[LOG_MSG_SIZE];

    while(true)
    {
        if (xQueueReceive(xLogQueue, msg, portMAX_DELAY) == pdPASS)
        {
            putsUart0(msg);   // single writer to UART
        }
    }
}

// Task for toggling yellow led every 500 milliseconds
void vBlinkYellowTask(void *pvParameters)
{
    while(true)
    {
        togglePinValue(YELLOW_LED);
#if (configUSE_PREEMPTION == 0)   // cooperative
        taskYIELD();              // must yield for other tasks
#endif
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

// Task for toggling red led every 500 milliseconds
void vBlinkRedTask(void *pvParameters)
{
    while (true)
    {
        togglePinValue(RED_LED);
#if (configUSE_PREEMPTION == 0)   // cooperative
        taskYIELD();              // must yield for other tasks
#endif
        vTaskDelay(pdMS_TO_TICKS(250));
    }
}

// Task for toggling green led every 500 milliseconds
void vBlinkGreenTask(void *pvParameters)
{
    while (true)
    {
        togglePinValue(GREEN_LED);   // Blink LED
#if (configUSE_PREEMPTION == 0)   // cooperative
        taskYIELD();              // must yield for other tasks
#endif
        vTaskDelay(pdMS_TO_TICKS(500)); // Blink every 500 ms
    }
}

// Timer for toggling on-board red Led to show system is running
void prvAutoReloadTimerCallback( TimerHandle_t xTimer )
{
    togglePinValue(RED_LED_BOARD);
}

// This function is for toggling an led to show every time the idle task is running
void vApplicationIdleHook(void)
{
    // Toggle LED here
    static uint32_t count = 0;

    count++;
    if (count >= 100000)
    {
        togglePinValue(WHITE_LED);   // Example: PF1 (red LED on TM4C123 LaunchPad)
        count = 0;
    }
}

// This task creates, resumes, suspends, and deletes one of the three led tasks based on which button press was detected
void vControllerTask(void *pvParameters)
{
    while(true)
    {
        uint8_t btn = readPbs();

        switch (btn)
        {
            case 1:  // PB1: Suspend Green LED Task
                if (xLed1Handle != NULL)
                    vTaskSuspend(xLed1Handle);
                break;

            case 2:  // PB2: Resume Green LED Task
                if (xLed1Handle != NULL)
                {
                    vTaskResume(xLed1Handle);
                    //vTaskDelete(xLed3Handle);
                    //xLed3Handle = NULL;
                }
                break;

            case 4:  // PB3: Create Red LED Task
                if (xLed2Handle == NULL)
                    xTaskCreate(vBlinkRedTask, "Red Led", 128, NULL, 2, &xLed2Handle);
                break;

            case 8: // PB4: Delete Red LED Task
                if (xLed2Handle != NULL)
                {
                    vTaskDelete(xLed2Handle);
                    xLed2Handle = NULL;
                }
                break;

            case 16: // PB5: Toggle Yellow LED Task
                if (xLed3Handle == NULL)
                {
                    xTaskCreate(vBlinkYellowTask, "Yellow Led", 128, NULL, 2, &xLed3Handle);
                }
                else
                {
                    vTaskDelete(xLed3Handle);
                    xLed3Handle = NULL;
                }
                break;

            case 32: // PB6: Restart LED1
                if (xLed1Handle != NULL)
                {
                    vTaskDelete(xLed1Handle);
                    xLed1Handle = NULL;
                }
                xTaskCreate(vBlinkGreenTask, "Green Led", 128, NULL, 2, &xLed1Handle);
                break;

            default:
                break;
        }

        vTaskDelay(pdMS_TO_TICKS(150)); // debounce + polling
    }
}

void vApplicationMallocFailedHook(void)
{
    // Runs if pvPortMalloc() fails
    putsUart0("ERROR: Malloc failed!\r\n");
    taskDISABLE_INTERRUPTS();
    while(true); // halt system, or reset if you prefer
}

void vMemoryTask(void *pvParameters)
{
    //char buf[128];

    while(true)
    {
        char buf[LOG_MSG_SIZE];
        int i;

        // Section header
        snprintf(buf, sizeof(buf), "Memory Information\n---------------------\n");
        xQueueSend(xLogQueue, buf, portMAX_DELAY);

        // Heap info
        snprintf(buf, sizeof(buf),
                 "Free Heap: %u bytes\n"
                 "Min Ever Free Heap: %u bytes\n"
                 "Shell Stack Remaining: %u bytes\n\n",
                 (unsigned) xPortGetFreeHeapSize(),
                 (unsigned) xPortGetMinimumEverFreeHeapSize(),
                 (unsigned) (uxTaskGetStackHighWaterMark(NULL) * sizeof(StackType_t)));
        xQueueSend(xLogQueue, buf, portMAX_DELAY);

        // Runtime stats
        char runtimeBuf[512];
        vTaskGetRunTimeStats(runtimeBuf);

        snprintf(buf, sizeof(buf), "Task CPU Usage (%%):\n");
        xQueueSend(xLogQueue, buf, portMAX_DELAY);

        // Break runtimeBuf into chunks of 100 chars to avoid queue overflow
        for (i = 0; i < strlen(runtimeBuf); i += 100)
        {
             strncpy(buf, &runtimeBuf[i], 100);
             buf[100] = '\0';
             xQueueSend(xLogQueue, buf, portMAX_DELAY);
        }

        // Number of tasks
        snprintf(buf, sizeof(buf), "Number of tasks: %u\n\n",
                 (unsigned) uxTaskGetNumberOfTasks());
        xQueueSend(xLogQueue, buf, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

void vStackMonitorTask(void *pvParameters)
{
    static TaskStatus_t pxTaskStatusArray[10];
    UBaseType_t uxArraySize;
    uint32_t ulTotalRunTime;
    int i;

    while(true)
    {
        uxArraySize = uxTaskGetSystemState(pxTaskStatusArray, 10, &ulTotalRunTime);

        for (i = 0; i < uxArraySize; i++)
        {
            putsUart0("\n");
            snprintf(string, sizeof(string), "Task: %s, Stack Left: %lu words\n",
                   pxTaskStatusArray[i].pcTaskName,
                   pxTaskStatusArray[i].usStackHighWaterMark);
            putsUart0(string);
            putsUart0("\n");
        }

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

void vMemTestTask(void *pvParameters)
{
    void *ptrs[10];
    int i;

    while (1)
    {
        // Allocate different block sizes
        for (i = 0; i < 10; i++)
        {
            ptrs[i] = pvPortMalloc(100 * (i+1));
            if (ptrs[i] == NULL)
                putsUart0("Allocation failed!\r\n");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));

        // Free half of them
        for (i = 0; i < 10; i += 2)
        {
            vPortFree(ptrs[i]);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));

        // Free the rest
        for (i = 1; i < 10; i += 2)
        {
            vPortFree(ptrs[i]);
        }

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void vShellTask(void *pvParameters)
{
    USER_DATA data;
    int count = 0;
    uint8_t i;

    memset(&data, 0, sizeof(data));
    putsUart0("\r\n> ");   // show initial prompt

    while (true)
    {
        if (kbhitUart0())
        {
            char c = getcUart0();

            if ((c == 8 || c == 127) && count > 0)   // backspace
            {
                count--;
            }
            else if (c == 13) // Enter
            {
                data.buffer[count] = 0;   // null terminate
                parseFields(&data);

                for (i = 0; i < data.fieldCount; i++)
                {
                    putcUart0(data.fieldType[i]);
                    putcUart0('\t');
                    putsUart0(&data.buffer[data.fieldPosition[i]]);
                    putcUart0('\n');
                }

                if (isCommand(&data, "Memory Info", 0))
                {
                    char buf[LOG_MSG_SIZE];
                    int i;

                    // Section header
                    snprintf(buf, sizeof(buf), "Memory Information\n---------------------\n");
                    xQueueSend(xLogQueue, buf, portMAX_DELAY);

                    // Heap info
                    snprintf(buf, sizeof(buf),
                             "Free Heap: %u bytes\n"
                             "Min Ever Free Heap: %u bytes\n"
                             "Shell Stack Remaining: %u bytes\n\n",
                             (unsigned) xPortGetFreeHeapSize(),
                             (unsigned) xPortGetMinimumEverFreeHeapSize(),
                             (unsigned) (uxTaskGetStackHighWaterMark(NULL) * sizeof(StackType_t)));
                    xQueueSend(xLogQueue, buf, portMAX_DELAY);

                    // Runtime stats
                    char runtimeBuf[512];
                    vTaskGetRunTimeStats(runtimeBuf);

                    snprintf(buf, sizeof(buf), "Task CPU Usage (%%):\n");
                    xQueueSend(xLogQueue, buf, portMAX_DELAY);

                    // Break runtimeBuf into chunks of 100 chars to avoid queue overflow
                    for (i = 0; i < strlen(runtimeBuf); i += 100)
                    {
                         strncpy(buf, &runtimeBuf[i], 100);
                         buf[100] = '\0';
                         xQueueSend(xLogQueue, buf, portMAX_DELAY);
                    }

                    // Number of tasks
                    snprintf(buf, sizeof(buf), "Number of tasks: %u\n\n", (unsigned) uxTaskGetNumberOfTasks());
                    xQueueSend(xLogQueue, buf, portMAX_DELAY);
                }
                else if(isCommand(&data, "Green Delete", 0))
                {
                    xSemaphoreTake(xUartMutex, portMAX_DELAY);
                    vTaskDelete(xLed1Handle);
                    xLed1Handle = NULL;
                    xSemaphoreGive(xUartMutex);
                }
                else if(isCommand(&data, "Delete Yellow", 0))
                {
                    xSemaphoreTake(xUartMutex, portMAX_DELAY);
                    vTaskDelete(xLed3Handle);
                    xLed3Handle = NULL;
                    xSemaphoreGive(xUartMutex);
                }
                else if (isCommand(&data, "Preempt", 1))
                {
                    if (strcmp(getFieldString(&data, 1), "on") == 0)
                    {
                        xTaskResumeAll();   // allow preemption
                        putsUart0("Preemption enabled\n");
                    }
                    else if (strcmp(getFieldString(&data, 1), "off") == 0)
                    {
                        vTaskSuspendAll();  // tasks switch only when they yield
                        putsUart0("Preemption disabled (cooperative mode)\n");
                    }
                }

                else
                {
                    xSemaphoreTake(xUartMutex, portMAX_DELAY);
                    putsUart0("Invalid Command\r\n");
                    xSemaphoreGive(xUartMutex);
                }

                count = 0;   // reset buffer
                memset(data.buffer, 0, sizeof(data.buffer));
                putsUart0("> "); // show prompt again
            }
            else if (c >= 32 && count < MAX_CHARS-1)
            {
                data.buffer[count++] = c;
            }
        }

        vTaskDelay(1); // yield to other tasks
    }
}
