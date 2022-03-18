/** @file HL_sys_main.c 
*   @brief Application main file
*   @date 11-Dec-2018
*   @version 04.07.01
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/* 
* Copyright (C) 2009-2018 Texas Instruments Incorporated - www.ti.com  
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


/* USER CODE BEGIN (0) */
/* USER CODE END */

/* Include Files */

#include "HL_sys_common.h"

/* USER CODE BEGIN (1) */
#include "HL_system.h"
#include "HL_gio.h"
#include "HL_sci.h"

//FreeRTOS 관련 헤더 추가
#include "FreeRTOS.h"
#include "os_task.h"
#include "os_semphr.h"

#include <stdio.h>
#include <string.h>

#define UART sciREG1

void sci_display_text(sciBASE_t *sci, uint8 *text, uint32 length);
void sci_display_data(sciBASE_t *sci, uint8 *text, uint32 length);
void wait(uint32 time);

char txt_buf[256] = {0};
unsigned int buf_len;
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
SemaphoreHandle_t sem;

TaskHandle_t led1_fd;
TaskHandle_t led2_fd;
TaskHandle_t led3_fd;
TaskHandle_t uart_fd;

void led1_task(void *pvParameters);
void led2_task(void *pvParameters);
void led3_task(void *pvParameters);
void uart_task(void *pvParameters);

void init_peripheral(void)
{
    sciInit();
    sprintf(txt_buf, "SCI Init Success!\n\r\0");
    buf_len = strlen(txt_buf);
    sci_display_text(UART, (uint8 *)txt_buf, buf_len);

    gioInit();
    sprintf(txt_buf, "GIO Init Success!\n\r\0");
    buf_len = strlen(txt_buf);
    sci_display_text(UART, (uint8 *)txt_buf, buf_len);
}
/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
    init_peripheral();

    //semaphore는 BUS 데드락을 방지하기 위해
    //task
    sem = xSemaphoreCreateBinary();
    xSemaphoreGive(sem);

    //task 생성
    if(xTaskCreate(led1_task, "led1", configMINIMAL_STACK_SIZE,
                   NULL, 5, &led1_fd) != pdPASS)
    {
        for(;;)
            ;
    }
    if(xTaskCreate(led2_task, "led2", configMINIMAL_STACK_SIZE,
                   NULL, 5, &led2_fd) != pdPASS)
    {
        for(;;)
            ;
    }
    if(xTaskCreate(led3_task, "led3", configMINIMAL_STACK_SIZE,
                   NULL, 5, &led3_fd) != pdPASS)
    {
        for(;;)
            ;
    }
    if(xTaskCreate(uart_task, "uart", configMINIMAL_STACK_SIZE*2,
                   NULL, 5, &uart_fd) != pdPASS)
    {
        for(;;)
            ;
    }

    //task scheduler enable
    vTaskStartScheduler();
/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
void led1_task(void *pvParameters)
{
    for(;;)
    {
        if(xSemaphoreTake(sem, (TickType_t)0x01) == pdTRUE)
        {
            gioToggleBit(gioPORTB, 7);

            xSemaphoreGive(sem);

            vTaskDelay(500);
        }
    }
}
void led2_task(void *pvParameters)
{
    for(;;)
    {
        if(xSemaphoreTake(sem, (TickType_t)0x01) == pdTRUE)
        {
            gioToggleBit(gioPORTB, 6);

            xSemaphoreGive(sem);

            vTaskDelay(750);
        }
    }
}
void led3_task(void *pvParameters)
{
    for(;;)
    {
        if(xSemaphoreTake(sem, (TickType_t)0x01) == pdTRUE)
        {
            gioToggleBit(gioPORTB, 5);

            xSemaphoreGive(sem);

            vTaskDelay(625);
        }
    }
}
void uart_task(void *pvParameters)
{
    for(;;)
    {
        if(xSemaphoreTake(sem, (TickType_t)0x01) == pdTRUE)
        {
            sprintf(txt_buf, "EDDI Robot Academy!\n\r\0");
            buf_len = strlen(txt_buf);
            sci_display_text(UART, (uint8 *)txt_buf, buf_len);

            xSemaphoreGive(sem);

            vTaskDelay(250);
        }
    }
}
void sci_display_text(sciBASE_t *sci, uint8 *text, uint32 length)
{
    while(length--)
    {
        while((UART->FLR & 0x4) == 4);
        sciSendByte(UART, *text++);
    }
}
void sci_display_data(sciBASE_t *sci, uint8 *text, uint32 length)
{
    while(length--)
    {
        while((UART->FLR & 0x4) == 4);
        sciSendByte(UART, *text);
    }
}
void wait(uint32 time)
{
    //표준 C는 for문안에서 타입 선언이 안됨
    int i;
    for(i = 0; i < time; i++);
}
/* USER CODE END */
