// This file contains all the function prototypes for the app.c file

#ifndef APP_H
#define APP_H

#include "qpc.h"
#include "ir_controller.h"
#include "robot_controller.h"
#include "servo_controller.h"

void App_Init(void);
int App_Run(void);

// QPC event signals
enum EventSignals {
	OBJECT_DETECTED_SIG = Q_USER_SIG,
    MOVE_SERVO_SIG,
	SERVO_TICK_SIG,
    SERVO_DONE_SIG,
	POLL_SIG,
	TICK_SIG
};

// Servo movement event
typedef struct {
	QEvt super;
	uint16_t angle[4];
} ServoMoveEvt;

#endif
