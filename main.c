//*****************************************************************************
//
// main.c - The entry point for the application that demonstrates using tiva
// sdk driverlib and freertos together on a tiva board
//
// Author: Abhinash Khanal
// Created: 03-23-2023
// Last Updated: 03-02-2023
//
//*****************************************************************************

#include <board.h>
#include <driverlib/gpio.h>
#include <FreeRTOS.h>
#include <inc/hw_memmap.h>
#include <stdint.h>
#include <task.h>
#include <uart_rtos.h>

#define init_TASK_PRIORITY 3
#define init_TASK_LABEL "init"

static bool s_led = false;

static void vInitTask(void *pvParameters);

//*****************************************************************************
//
// This example demonstrates how to send a string of data to the UART.
//
//*****************************************************************************
int main(void)
{
    board_init();

    xTaskCreate(vInitTask, init_TASK_LABEL, configMINIMAL_STACK_SIZE, NULL, init_TASK_PRIORITY, NULL);
    vTaskStartScheduler();

    // Control should never reach here since the scheduler has been started

    while (1)
        ;
}

static void vInitTask(void *pvParameters)
{
    for (;;)
    {
        UARTSend((uint8_t *)"Hello, world!\n\r", 16);
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, s_led ? 0 : 1);
        s_led = !s_led;
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
