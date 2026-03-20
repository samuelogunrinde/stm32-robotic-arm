#include "servo_controller.h"
#include "robot_controller.h"
#include "bsp.h"
#include "servo.h"

// Active object constructor
void ServoController_ctor(ServoController * const me) {
	// initialize the active object
    QActive_ctor(&me->super, Q_STATE_CAST(&ServoController_initial));
    // construct the time event used for motion completion
    QTimeEvt_ctorX(&me->servoTimer, &me->super, SERVO_DONE_SIG, 0U);
}

// Active object initial state
QState ServoController_initial(ServoController * const me, void const * const par) {
	// Initialize servo driver
    Servo_Init();
    // transition to the idle state
    return Q_TRAN(&ServoController_idle);
}

QState ServoController_idle(ServoController * const me, QEvt const * const e) {
	QState status_;
	switch (e->sig) {
        case MOVE_SERVO_SIG: {

        	// Turn servo predefined movements into PWM servo control
            ServoMoveEvt const *evt = (ServoMoveEvt const *)e;

            for (uint8_t i = 0; i < 4; i++) {
                Servo_SetAngle(i, evt->angle[i]);
            }

            // Start periodic update (20ms)
            QTimeEvt_armX(&me->servoTimer, BSP_TICKS_PER_SEC/50, BSP_TICKS_PER_SEC/50);
            status_ = Q_HANDLED();
            break;
        }
        case SERVO_DONE_SIG: {

        	// Motion control
        	Servo_Update(0.02f);

        	if (Servo_IsAtTarget()) {
        		QTimeEvt_disarm(&me->servoTimer);

        		// Post servo movement done signal to RobotController active object
        		QACTIVE_POST(AO_RobotController, Q_NEW(QEvt, SERVO_DONE_SIG), me);
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
