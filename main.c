//*****************************************************************************
//
// uart_echo.c - Example for reading data from and writing data to the UART in
//               an interrupt driven fashion.
//
// Copyright (c) 2013-2020 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
//
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
//
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
//
// This is part of revision 2.2.0.295 of the EK-TM4C129EXL Firmware Package.
//
//*****************************************************************************

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <stdint.h>
#include <board.h>
#include <uart_rtos.h>

#define mainCHECK_TASK_PRIORITY 3

static void vPrintTask(void *pvParameters);

void *memcpy(void *dst, void *src, size_t len)
{
    uint8_t *_dst = (uint8_t *)dst;
    uint8_t *_src = (uint8_t *)src;
    while (len--)
    {
        *(_src++) = *(_dst++);
    }
    return NULL;
}

void *memset(void *dst, int c, size_t len)
{
    uint8_t _c = c & 0xff;
    uint8_t *_dst = (uint8_t *)dst;
    while (len--)
    {
        *(_dst++) = _c;
    }
    return NULL;
}

//*****************************************************************************
//
// This example demonstrates how to send a string of data to the UART.
//
//*****************************************************************************
int main(void)
{
    board_init();

    xTaskCreate(vPrintTask, "Check", configMINIMAL_STACK_SIZE, NULL, mainCHECK_TASK_PRIORITY, NULL);
    vTaskStartScheduler();

    //
    // Loop forever echoing data through the UART.
    //
    while (1)
    {
    }
}

static void vPrintTask(void *pvParameters)
{
    //
    // Prompt for text to be entered.
    //
    UARTSend((uint8_t *)"\033[2JEnter text: ", 16);
    for (;;)
    {
        UARTSend((uint8_t *)"Hello, world!\n\r", 16);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
