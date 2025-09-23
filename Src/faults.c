/*
 * faults.c
 *
 *  Created on: Sep 18, 2025
 *      Author: Omar Elsaghir
 */
#include <inttypes.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "faults.h"
#include "gpio.h"
#include "uart0.h"

// Hard Fault ISR
void HardFaultISR(void)
{
    char str[100];
    uint32_t *stacked = (uint32_t *)__get_MSP();  // pointer to stacked registers

    putsUart0("\r\n--- HARDFAULT ---\r\n");
    snprintf(str, sizeof(str), "R0  = 0x%08lX\r\n", stacked[0]);
    putsUart0(str);
    snprintf(str, sizeof(str), "R1  = 0x%08lX\r\n", stacked[1]);
    putsUart0(str);
    snprintf(str, sizeof(str), "R2  = 0x%08lX\r\n", stacked[2]);
    putsUart0(str);
    snprintf(str, sizeof(str), "R3  = 0x%08lX\r\n", stacked[3]);
    putsUart0(str);
    snprintf(str, sizeof(str), "R12 = 0x%08lX\r\n", stacked[4]);
    putsUart0(str);
    snprintf(str, sizeof(str), "LR  = 0x%08lX\r\n", stacked[5]);
    putsUart0(str);
    snprintf(str, sizeof(str), "PC  = 0x%08lX\r\n", stacked[6]);
    putsUart0(str);
    snprintf(str, sizeof(str), "xPSR= 0x%08lX\r\n", stacked[7]);
    putsUart0(str);
/*
    // Fault status registers
    snprintf(str, sizeof(str), "HFSR = 0x%08lX\r\n", SCB->HFSR);
    putsUart0(str);
    snprintf(str, sizeof(str), "CFSR = 0x%08lX\r\n", SCB->CFSR);
    putsUart0(str);
    snprintf(str, sizeof(str), "MMFAR= 0x%08lX\r\n", SCB->MMFAR);
    putsUart0(str);
    snprintf(str, sizeof(str), "BFAR = 0x%08lX\r\n", SCB->BFAR);
    putsUart0(str);
*/
    while(true); // stop here
}

// Bus Fault ISR
void BusFaultISR(void)
{
    char str[100];
    uint32_t *stacked = (uint32_t *)__get_MSP();  // pointer to stacked registers

    putsUart0("\r\n--- BUS FAULT ---\r\n");
    snprintf(str, sizeof(str), "R0  = 0x%08lX\r\n", stacked[0]);
    putsUart0(str);
    snprintf(str, sizeof(str), "R1  = 0x%08lX\r\n", stacked[1]);
    putsUart0(str);
    snprintf(str, sizeof(str), "R2  = 0x%08lX\r\n", stacked[2]);
    putsUart0(str);
    snprintf(str, sizeof(str), "R3  = 0x%08lX\r\n", stacked[3]);
    putsUart0(str);
    snprintf(str, sizeof(str), "R12 = 0x%08lX\r\n", stacked[4]);
    putsUart0(str);
    snprintf(str, sizeof(str), "LR  = 0x%08lX\r\n", stacked[5]);
    putsUart0(str);
    snprintf(str, sizeof(str), "PC  = 0x%08lX\r\n", stacked[6]);
    putsUart0(str);
    snprintf(str, sizeof(str), "xPSR= 0x%08lX\r\n", stacked[7]);
    putsUart0(str);

    while(true); // stop here
}

// MPU Fault ISR
void MpuFaultISR(void)
{
    char str[100];
    uint32_t *stacked = (uint32_t *)__get_MSP();  // pointer to stacked registers

    putsUart0("\r\n--- MPU FAULT ---\r\n");
    snprintf(str, sizeof(str), "R0  = 0x%08lX\r\n", stacked[0]);
    putsUart0(str);
    snprintf(str, sizeof(str), "R1  = 0x%08lX\r\n", stacked[1]);
    putsUart0(str);
    snprintf(str, sizeof(str), "R2  = 0x%08lX\r\n", stacked[2]);
    putsUart0(str);
    snprintf(str, sizeof(str), "R3  = 0x%08lX\r\n", stacked[3]);
    putsUart0(str);
    snprintf(str, sizeof(str), "R12 = 0x%08lX\r\n", stacked[4]);
    putsUart0(str);
    snprintf(str, sizeof(str), "LR  = 0x%08lX\r\n", stacked[5]);
    putsUart0(str);
    snprintf(str, sizeof(str), "PC  = 0x%08lX\r\n", stacked[6]);
    putsUart0(str);
    snprintf(str, sizeof(str), "xPSR= 0x%08lX\r\n", stacked[7]);
    putsUart0(str);

    while(true); // stop here
}

// Usage Fault ISR
void UsageFaultISR(void)
{
    char str[100];
    uint32_t *stacked = (uint32_t *)__get_MSP();  // pointer to stacked registers

    putsUart0("\r\n--- USAGE FAULT ---\r\n");
    snprintf(str, sizeof(str), "R0  = 0x%08lX\r\n", stacked[0]);
    putsUart0(str);
    snprintf(str, sizeof(str), "R1  = 0x%08lX\r\n", stacked[1]);
    putsUart0(str);
    snprintf(str, sizeof(str), "R2  = 0x%08lX\r\n", stacked[2]);
    putsUart0(str);
    snprintf(str, sizeof(str), "R3  = 0x%08lX\r\n", stacked[3]);
    putsUart0(str);
    snprintf(str, sizeof(str), "R12 = 0x%08lX\r\n", stacked[4]);
    putsUart0(str);
    snprintf(str, sizeof(str), "LR  = 0x%08lX\r\n", stacked[5]);
    putsUart0(str);
    snprintf(str, sizeof(str), "PC  = 0x%08lX\r\n", stacked[6]);
    putsUart0(str);
    snprintf(str, sizeof(str), "xPSR= 0x%08lX\r\n", stacked[7]);
    putsUart0(str);

    while(true); // stop here
}
