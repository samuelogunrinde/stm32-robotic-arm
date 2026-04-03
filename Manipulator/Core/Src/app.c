// Initialization
#include "qpc.h"
#include "bsp.h"
#include "app.h"

// event pool
// small events
static QF_MPOOL_EL(QEvt) smallEvtPool[20];
// servo movement events
static QF_MPOOL_EL(ServoMoveEvt) servoEvtPool[10];

static QEvt const *RobotController_queue[10]; // Memory buffer for the private event queue of the active object
static RobotController robotcontroller; // Instance of the active object
QActive *AO_RobotController = &robotcontroller.super; // Pointer to the Active Object Super class

static QEvt const *ServoController_queue[10]; // Memory buffer for the private event queue of the active object
static ServoController servocontroller; // Instance of the active object
QActive *AO_ServoController = &servocontroller.super; // Pointer to the Active Object Super class

static QEvt const *IRSensor_queue[10]; // Memory buffer for the private event queue of the active object
static IRSensor irsensor; // Instance of the active object
QActive *AO_IRSensor = &irsensor.super; // Pointer to the Active Object Super class

void App_Init(void) {

	// Initialize QPC
	QF_init();

	// initialize the event pools
	QF_poolInit(smallEvtPool, sizeof(smallEvtPool), sizeof(smallEvtPool[0]));
	QF_poolInit(servoEvtPool, sizeof(servoEvtPool), sizeof(servoEvtPool[0]));

	// create active objects and start
	RobotController_ctor(&robotcontroller);
	ServoController_ctor(&servocontroller);
	IRSensor_ctor(&irsensor);

	QACTIVE_START(AO_RobotController, 1U, RobotController_queue,
			sizeof(RobotController_queue) / sizeof(RobotController_queue[0]),
			(void* )0, 0U, (void* )0);
	QACTIVE_START(AO_ServoController, 2U, ServoController_queue,
			sizeof(ServoController_queue) / sizeof(ServoController_queue[0]),
			(void* )0, 0U, (void* )0);
	QACTIVE_START(AO_IRSensor, 3U, IRSensor_queue,
			sizeof(IRSensor_queue) / sizeof(IRSensor_queue[0]), (void* )0, 0U,
			(void* )0);
}

int App_Run(void) {
	return QF_run();
}
