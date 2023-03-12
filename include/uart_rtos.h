#ifndef __UART_RTOS_H__
#define __UART_RTOS_H__

#include <stdint.h>

void UARTIntHandler(void);
void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count);

#endif