#include "app.h"
#include "bsp.h"
#include "inverse_kinematics.h"

// Set servo angles
void sendMove(RobotController *me, const uint16_t move[4]) {
	// Allocate an event object from an event pool
	ServoMoveEvt *evt = Q_NEW(ServoMoveEvt, MOVE_SERVO_SIG);

	for (int i = 0; i < 4; i++) {
		evt->angle[i] = move[i];
	}

	// Post desired movement (event) to the Servo Active Object queue
	QACTIVE_POST(AO_ServoController, &evt->super, me);
}

// Generate servo angles from given positions
void moveToXYZ(RobotController *me, float x, float y, float z, uint16_t grip) {

    uint16_t angles[4];

    if (IK_Solve(x, y, z, angles)) {

        angles[3] = grip;   // gripper independent

        sendMove(me, angles);
    }
}

// Log servo angles to the UART serial terminal
void logAngles(){
	float angles[4];
	Servo_GetAngles(angles);

	Log_Print("Base: %.1f Shoulder: %.1f Elbow: %.1f Gripper: %.1f\r\n",
			angles[0], angles[1], angles[2], angles[3]);
}

// Active object constructor
void RobotController_ctor(RobotController * const me) {
	// initialize the Active object
	QActive_ctor(&me->super, Q_STATE_CAST(&Robot_initial));

	QTimeEvt_ctorX(&me->tickEvt, &me->super, TICK_SIG, 0U);
}

// Active object initial state
QState Robot_initial(RobotController * const me, void const * const par) {
    return Q_TRAN(&Robot_idle);
}

// FSM super state, triggers when manipulator is doing work
QState Robot_active(RobotController * const me, QEvt const * const e) {
    QState status_;
    switch(e->sig) {
        case Q_ENTRY_SIG: {
            Log_Print("Robot ACTIVE\r\n");
            QTimeEvt_armX(&me->tickEvt, BSP_TICKS_PER_SEC / 10, BSP_TICKS_PER_SEC / 10);
            status_ = Q_HANDLED();
            break;
        }
        case TICK_SIG: {
            BSP_toggleLed();
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
        case Q_EXIT_SIG: {
            QTimeEvt_disarm(&me->tickEvt);
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

QState Robot_idle(RobotController * const me, QEvt const * const e) {
	QState status_;
	switch(e->sig) {
		case Q_ENTRY_SIG: {
			BSP_ledGreenOn();
			status_ = Q_HANDLED();
			break;
		}
		case OBJECT_DETECTED_SIG: {
			// transition to the next state
			status_ = Q_TRAN(&Robot_move_to_pick);
			break;
		}
		case Q_EXIT_SIG: {
			BSP_ledGreenOff();
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

QState Robot_move_to_pick(RobotController * const me, QEvt const * const e) {
	QState status_;
	switch(e->sig) {
		case Q_ENTRY_SIG: {
			Log_Print("State: MOVE_TO_PICK\r\n");
			// post servo movement event
			moveToXYZ(me, PICK.x, PICK.y, PICK.z, GRIP_OPEN);
			status_ = Q_HANDLED();
			break;
		}
		case SERVO_DONE_SIG: {
			logAngles();
			// transition to the next state
			status_ = Q_TRAN(&Robot_grip);
			break;
		}
		default: {
			status_ = Q_SUPER(&Robot_active);
			break;
		}
	}
	return status_;
}

QState Robot_grip(RobotController * const me, QEvt const * const e) {
	QState status_;
	switch(e->sig) {
		case Q_ENTRY_SIG: {
			Log_Print("State: GRIP\r\n");
			// post servo movement event
			moveToXYZ(me, PICK.x, PICK.y, PICK.z, GRIP_HOLD);
			status_ = Q_HANDLED();
			break;
		}
		case SERVO_DONE_SIG: {
			logAngles();
			// transition to the next state
			status_ = Q_TRAN(&Robot_lift);
			break;
		}
		default: {
			status_ = Q_SUPER(&Robot_active);
			break;
		}
	}
	return status_;
}

QState Robot_lift(RobotController * const me, QEvt const * const e) {
	QState status_;
	switch(e->sig) {
		case Q_ENTRY_SIG: {
			Log_Print("State: LIFT\r\n");
			// post servo movement event
			moveToXYZ(me, LIFT.x, LIFT.y, LIFT.z, GRIP_HOLD);
			status_ = Q_HANDLED();
			break;
		}
		case SERVO_DONE_SIG: {
			logAngles();
			// transition to the next state
			status_ = Q_TRAN(&Robot_move_to_place);
			break;
		}
		default: {
			status_ = Q_SUPER(&Robot_active);
			break;
		}
	}
	return status_;
}

QState Robot_move_to_place(RobotController * const me, QEvt const * const e) {
	QState status_;
	switch(e->sig) {
		case Q_ENTRY_SIG: {
			Log_Print("State: MOVE_TO_PLACE\r\n");
			// post servo movement event
			moveToXYZ(me, PLACE.x, PLACE.y, PLACE.z, GRIP_HOLD);
			status_ = Q_HANDLED();
			break;
		}
		case SERVO_DONE_SIG: {
			logAngles();
			// transition to the next state
			status_ = Q_TRAN(&Robot_release);
			break;
		}
		default: {
			status_ = Q_SUPER(&Robot_active);
			break;
		}
	}
	return status_;
}

QState Robot_release(RobotController * const me, QEvt const * const e) {
	QState status_;
	switch(e->sig) {
		case Q_ENTRY_SIG: {
			Log_Print("State: RELEASE\r\n");
			// post servo movement event
			moveToXYZ(me, PLACE.x, PLACE.y, PLACE.z, GRIP_OPEN);
			status_ = Q_HANDLED();
			break;
		}
		case SERVO_DONE_SIG: {
			logAngles();
			// transition to the next state
			status_ = Q_TRAN(&Robot_return_home);
			break;
		}
		default: {
			status_ = Q_SUPER(&Robot_active);
			break;
		}
	}
	return status_;
}

QState Robot_return_home(RobotController * const me, QEvt const * const e) {
	QState status_;
	switch(e->sig) {
		case Q_ENTRY_SIG: {
			Log_Print("State: RETURN HOME\r\n");
			// post servo movement event
			moveToXYZ(me, HOME.x, HOME.y, HOME.z, GRIP_CLOSE);
			status_ = Q_HANDLED();
			break;
		}
		case SERVO_DONE_SIG: {
			logAngles();
			// transition back to the idle state
			status_ = Q_TRAN(&Robot_idle);
			break;
		}
		default: {
			status_ = Q_SUPER(&Robot_active);
			break;
		}
	}
	return status_;
}

