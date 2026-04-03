#include "bsp.h"
#include "qpc.h"

// Initialize Board Support Package
void BSP_Init(void) {
	// Initialize gpio pins
	MX_GPIO_Init();
	// Initialize timer2 for pulse width modulation (PWM)
	MX_TIM2_Init();
	// Initialize timer3 for 38kHz infrared (IR) transmission
	MX_TIM3_Init();
	// Initialize UART communication
	MX_USART2_UART_Init();
}

// Green LED ON
void BSP_ledGreenOn(void) {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
}

// Green LED OFF
void BSP_ledGreenOff(void) {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
}

// Toggle green LED
void BSP_toggleLed(void) {
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}


void QF_onStartup(void) {
    SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC);
}

void QF_onCleanup(void) {
}

void QV_onIdle(void) {
#ifdef NDEBUG
    /* Put the CPU and peripherals to the low-power mode.
    * you might need to customize the clock management for your application,
    * see the datasheet for your particular Cortex-M MCU.
    */
    QV_CPU_SLEEP();  /* atomically go to sleep and enable interrupts */
#else
    QF_INT_ENABLE(); /* just enable interrupts */
#endif
}

void Q_onError(char const *module, int_t id){
	(void)module;
	(void)id;

	while(1){

	}
}
