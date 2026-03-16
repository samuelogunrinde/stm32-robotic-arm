// This file contains all the function prototypes for the robot_controller.c file

#ifndef ROBOT_CONTROLLER_H
#define ROBOT_CONTROLLER_H

#include "qpc.h"

// Active objects
typedef struct {
	QActive super;
} RobotController;

// Function prototypes
void sendMove(RobotController *me, const uint16_t move[4]);
void RobotController_ctor(RobotController * const me);
QState Robot_initial(RobotController * const me, void const * const par);
QState Robot_idle(RobotController * const me, QEvt const * const e);
QState Robot_move_to_pick(RobotController * const me, QEvt const * const e);
QState Robot_grip(RobotController * const me, QEvt const * const e);
QState Robot_lift(RobotController * const me, QEvt const * const e);
QState Robot_move_to_place(RobotController * const me, QEvt const * const e);
QState Robot_release(RobotController * const me, QEvt const * const e);
QState Robot_return_home(RobotController * const me, QEvt const * const e);

// Active object pointer
extern QActive *AO_RobotController;

#endif
