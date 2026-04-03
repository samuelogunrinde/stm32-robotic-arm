// This file contains all the function prototypes for the bsp.c file
#ifndef BSP_H
#define BSP_H

#include "qpc.h"
#include "main.h"
#include "tim.h"
#include "gpio.h"
#include "servo.h"
#include "ir_sensor.h"
#include "usart.h"
#include "log.h"

#define BSP_TICKS_PER_SEC 1000U

void BSP_Init(void);
void BSP_ledGreenOn();
void BSP_ledGreenOff();
void BSP_toggleLed(void);

#endif
