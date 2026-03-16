#include "robot_controller.h"
#include "servo_controller.h"
#include "bsp.h"
#include "servo_moves.h"

void sendMove(RobotController *me, const uint16_t move[4]) {
	// Allocate an event object from an event pool
	ServoMoveEvt *evt = Q_NEW(ServoMoveEvt, MOVE_SERVO_SIG);

	for (int i = 0; i < 4; i++) {
		evt->angle[i] = move[i];
	}

	// Post desired movement (event) to the Servo Active Object queue
	QACTIVE_POST(AO_ServoController, &evt->super, me);
}

// Active object constructor
void RobotController_ctor(RobotController * const me) {
	// initialize the Active object
	QActive_ctor(&me->super, Q_STATE_CAST(&Robot_initial));
}

// Active object initial state
QState Robot_initial(RobotController * const me, void const * const par) {
    return Q_TRAN(&Robot_idle);
}

QState Robot_idle(RobotController * const me, QEvt const * const e) {
	QState status_;
	switch(e->sig) {
		case Q_ENTRY_SIG: {
			status_ = Q_HANDLED();
			break;
		}
		case OBJECT_DETECTED_SIG: {
			// transition to the next state
			status_ = Q_TRAN(&Robot_move_to_pick);
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
			// post servo movement event
			sendMove(me, MOVE_PICK);
			status_ = Q_HANDLED();
			break;
		}
		case SERVO_DONE_SIG: {
			// transition to the next state
			status_ = Q_TRAN(&Robot_grip);
			break;
		}
		default: {
			status_ = Q_SUPER(&QHsm_top);
			break;
		}
	}
	return status_;
}

QState Robot_grip(RobotController * const me, QEvt const * const e) {
	QState status_;
	switch(e->sig) {
		case Q_ENTRY_SIG: {
			// post servo movement event
			sendMove(me, MOVE_GRIP);
			status_ = Q_HANDLED();
			break;
		}
		case SERVO_DONE_SIG: {
			// transition to the next state
			status_ = Q_TRAN(&Robot_lift);
			break;
		}
		default: {
			status_ = Q_SUPER(&QHsm_top);
			break;
		}
	}
	return status_;
}

QState Robot_lift(RobotController * const me, QEvt const * const e) {
	QState status_;
	switch(e->sig) {
		case Q_ENTRY_SIG: {
			// post servo movement event
			sendMove(me, MOVE_LIFT);
			status_ = Q_HANDLED();
			break;
		}
		case SERVO_DONE_SIG: {
			// transition to the next state
			status_ = Q_TRAN(&Robot_move_to_place);
			break;
		}
		default: {
			status_ = Q_SUPER(&QHsm_top);
			break;
		}
	}
	return status_;
}

QState Robot_move_to_place(RobotController * const me, QEvt const * const e) {
	QState status_;
	switch(e->sig) {
		case Q_ENTRY_SIG: {
			// post servo movement event
			sendMove(me, MOVE_PLACE);
			status_ = Q_HANDLED();
			break;
		}
		case SERVO_DONE_SIG: {
			// transition to the next state
			status_ = Q_TRAN(&Robot_release);
			break;
		}
		default: {
			status_ = Q_SUPER(&QHsm_top);
			break;
		}
	}
	return status_;
}

QState Robot_release(RobotController * const me, QEvt const * const e) {
	QState status_;
	switch(e->sig) {
		case Q_ENTRY_SIG: {
			// post servo movement event
			sendMove(me, MOVE_RELEASE);
			status_ = Q_HANDLED();
			break;
		}
		case SERVO_DONE_SIG: {
			// transition to the next state
			status_ = Q_TRAN(&Robot_return_home);
			break;
		}
		default: {
			status_ = Q_SUPER(&QHsm_top);
			break;
		}
	}
	return status_;
}

QState Robot_return_home(RobotController * const me, QEvt const * const e) {
	QState status_;
	switch(e->sig) {
		case Q_ENTRY_SIG: {
			// post servo movement event
			sendMove(me, MOVE_HOME);
			status_ = Q_HANDLED();
			break;
		}
		case SERVO_DONE_SIG: {
			// transition back to the idle state
			status_ = Q_TRAN(&Robot_idle);
			break;
		}
		default: {
			status_ = Q_SUPER(&QHsm_top);
			break;
		}
	}
	return status_;
}

