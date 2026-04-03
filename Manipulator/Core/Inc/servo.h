// This file contains all the function prototypes for the servo.c file

#ifndef SERVO_H
#define SERVO_H

#include "main.h"
#include "tim.h"
#include <stdbool.h>

void Servo_Init(void);
void Servo_SetAngle(uint8_t servo_id, uint8_t angle);
void Servo_Update(float dt);
bool Servo_IsAtTarget(void);
void Servo_GetAngles(float *angles);

#endif

