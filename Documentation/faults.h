/*
 * faults.h
 *
 *  Created on: Sep 18, 2025
 *      Author: Omar Elsaghir
 */

#ifndef FAULTS_H_
#define FAULTS_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

void HardFaultISR(void);
void BusFaultISR(void);
void MpuFaultISR(void);
void UsageFaultISR(void);

#endif /* FAULTS_H_ */
