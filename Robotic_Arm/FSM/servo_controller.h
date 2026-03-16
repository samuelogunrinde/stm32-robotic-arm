// This file contains all the function prototypes for the servo_controller.c file

#ifndef SERVO_CONTROLLER_H
#define SERVO_CONTROLLER_H

#include "qpc.h"

// Active object
typedef struct {
	QActive super;
	QTimeEvt servoTimer;
} ServoController;

// Function prototypes
void ServoController_ctor(ServoController * const me);
QState ServoController_initial(ServoController * const me, void const * const par);
QState ServoController_idle(ServoController * const me, QEvt const * const e);

// Active object pointer
extern QActive *AO_ServoController;

#endif
