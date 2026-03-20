#include "bsp.h"
#include "ir_sensor.h"
#include "robot_controller.h"
#include "ir_controller.h"

// Active object constructor
void IRSensor_ctor(IRSensor * const me) {
	// initialize the Active object
    QActive_ctor(&me->super, Q_STATE_CAST(&IRSensor_initial));
    // construct the time event used for object detection
    QTimeEvt_ctorX(&me->pollTimer, &me->super, POLL_SIG, 0U);
}

// Active object initial state
QState IRSensor_initial(IRSensor * const me, void const * const par) {
	// Initialize ir sensor driver
    IR_Init();
    return Q_TRAN(&IRSensor_monitor);
}

// Polling state
QState IRSensor_monitor(IRSensor * const me, QEvt const * const e) {
	QState status_;
	switch (e->sig) {
		case Q_ENTRY_SIG: {

			me->detection_enabled = false;

			me->startup_count = 0;

			// sample sensor every 20 ms
			QTimeEvt_armX(&me->pollTimer, BSP_TICKS_PER_SEC/10, BSP_TICKS_PER_SEC/50);

			status_ = Q_HANDLED();
			break;
		}
		case POLL_SIG: {

			// Startup blanking
			if (!me->detection_enabled) {
				me->startup_count++;

				if (me->startup_count >= 500) {
					me->detection_enabled = true;
				}
				status_ = Q_HANDLED();
				break; // ignore object detection
			}

			// Object detection
			if (IR_Detected()) {
			// send detection event to RobotController active object
			QACTIVE_POST(AO_RobotController, Q_NEW(QEvt, OBJECT_DETECTED_SIG),
					me);
			}
			status_ = Q_HANDLED();
			break;
	}
		default: {
			status_ = Q_SUPER(&QHsm_top);
			break;
		}
	}
	return status_;
}
