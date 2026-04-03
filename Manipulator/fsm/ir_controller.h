// This file contains all the function prototypes for the ir_controller.c file

#ifndef IR_CONTROLLER_H
#define IR_CONTROLLER_H

#include "qpc.h"

// Active object
typedef struct {
	QActive super;
	QTimeEvt pollTimer;
	bool detection_enabled;
	uint16_t startup_count;
} IRSensor;

// Function prototypes
void IRSensor_ctor(IRSensor * const me);
QState IRSensor_initial(IRSensor * const me, void const * const par);
QState IRSensor_monitor(IRSensor * const me, QEvt const * const e);

// Active object pointer
extern QActive *AO_IRSensor;

#endif
