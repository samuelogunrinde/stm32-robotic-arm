#include "log.h"
#include "usart.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

// Log buffer
#define LOG_BUFFER_SIZE 128
static char log_buffer[LOG_BUFFER_SIZE];

// Log function
void Log_Print(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    vsnprintf(log_buffer, sizeof(log_buffer), fmt, args);

    va_end(args);

    // transmit
    HAL_UART_Transmit(&huart2, (uint8_t*)log_buffer, strlen(log_buffer), 10);
}
