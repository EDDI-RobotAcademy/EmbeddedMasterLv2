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
/* Include FreeRTOS scheduler files */
#include "FreeRTOS.h"
#include "os_semphr.h"
#include "os_task.h"

/* Include HET header file - types, definitions and function declarations for system driver */
#include "HL_het.h"
#include "HL_gio.h"
#include "HL_esm.h"
#include "HL_sci.h"
#include "HL_spi.h"

//#include <comm/prj_ethernet/include/prj_ethernet.h>
#include "prj_ethernet.h"
#include "gy_50_init.h"
#include "gy_50_en_intf.h"
#include "gy_50_rw.h"

#include "lwip/timers.h"

#include <string.h>
#include <stdio.h>

#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

void vTask1(void *pvParameters);
void vTask2(void *pvParameters);
void vTask3(void *pvParameters);

extern void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName );
extern uint32_t EMAC_LwIP_Main (uint8_t * emacAddress);

/* Define Task Handles */
xTaskHandle xTask1Handle;
xTaskHandle xTask2Handle;
xTaskHandle xTask3Handle;

/* Define semaphore Handles*/
SemaphoreHandle_t sem;

#define UART        sciREG1

void sci_display_text(sciBASE_t *sci, uint8_t *text,uint32_t length);
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
uint32_t ipAddr;
/* USER CODE END */

uint8	emacAddress[6U] = 	{0x00U, 0x08U, 0xeeU, 0x03U, 0xa6U, 0x6cU};
uint32 	emacPhyAddress	=	1U;

int main(void)
{
/* USER CODE BEGIN (3) */
    char buf[128];
    unsigned int buf_len;

    /*clear the ESM error manually*/
    esmREG->SR1[2] = 0xFFFFFFFFU;
    esmREG->SSR2   = 0xFFFFFFFF;
    esmREG->EKR = 0x0000000A;
    esmREG->EKR = 0x00000000;

    /* Set high end timer GIO port hetPort pin direction to all output */
    gioInit();
    gioSetDirection(gioPORTB, 0xFFFF);
    /* Set high end timer GIO port hetPort pin direction to all output */
    gioSetDirection(hetPORT1, 0xFFFFFFFF); //HDK uses NHET for LEDs

    sciInit();
    sprintf(buf, "SCI Init Success!\n\r\0");
    buf_len = strlen(buf);
    sci_display_text(UART, (uint8 *)buf, buf_len);

    init_spi_intf();
//    spiInit();

    sprintf(buf, "SPI Init Success!\n\r\0");
    buf_len = strlen(buf);
    sci_display_text(UART, (uint8 *)buf, buf_len);

    /*lwip Init*/
    ipAddr = EMAC_LwIP_Main (emacAddress);

    sem = xSemaphoreCreateBinary();
    xSemaphoreGive(sem);

    /* Create Task 1 */
    if (xTaskCreate(vTask1,"Task1", configMINIMAL_STACK_SIZE, NULL,
                    ((configMAX_PRIORITIES-1)|portPRIVILEGE_BIT), &xTask1Handle) != pdTRUE)
    {
        /* Task could not be created */
        while(1);
    }

    /* Create Task 2 */
    if (xTaskCreate(vTask2,"id_issue", configMINIMAL_STACK_SIZE, NULL, 2,
                    &xTask2Handle) != pdTRUE)
    {
        /* Task could not be created */
        while(1);
    }

    /* Create Task 3 */
    if (xTaskCreate(vTask3,"sensor_control", configMINIMAL_STACK_SIZE, NULL, 3,
                    &xTask3Handle) != pdTRUE)
    {
        /* Task could not be created */
        while(1);
    }
    /* Start Scheduler */
    vTaskStartScheduler();

    /* Run forever */
    while(1);
/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
/* Task1 */
void vTask1(void *pvParameters)
{
    uint32_t data[1] = {0};

    while(!udp_socket_handler());

    for(;;)
    {
        /* Taggle GIOB[6] with timer tick */
        gioSetBit(gioPORTB, 6, gioGetBit(gioPORTB, 6) ^ 1);
        /* Taggle HET[1] with timer tick */
        gioSetBit(hetPORT1, 17, gioGetBit(hetPORT1, 17) ^ 1);  //LED on HDK, top left

        taskENTER_CRITICAL();
        while(!id_issuance_handle((char *)"192.168.1.7", data, sizeof(data)/sizeof(uint32_t)));
        taskEXIT_CRITICAL();
        vTaskDelay(300);
    }
}

void vTask2(void *pvParameters)
{
    for(;;)
    {
        if(xSemaphoreTake(sem, (TickType_t)0x01) == pdTRUE)
        {
            /* Taggle GIOB[7] with timer tick */
            gioSetBit(gioPORTB, 7, gioGetBit(gioPORTB, 7) ^ 1);
            gioSetBit(hetPORT1, 18, gioGetBit(hetPORT1, 18) ^ 1);  //LED on HDK, bottom
            xSemaphoreGive(sem);
        }
        vTaskDelay(501);
    }
}

void vTask3(void *pvParameters)
{
    spiDAT1_t data_cfg;
    uint8_t whoami;
    for(;;)
    {
        if(xSemaphoreTake(sem, (TickType_t)0x01) == pdTRUE)
        {
            gy_50_en_spi(&data_cfg, SPI_FMT_0, 0xFE);
            whoami = gy_50_read(&data_cfg, (uint16_t)WHO_AM_I, 1);

            xSemaphoreGive(sem);
        }
        vTaskDelay(201);
    }
}

void sci_display_text(sciBASE_t *sci, uint8_t *text,uint32_t length)
{
    while(length--)
    {
        while ((sci->FLR & 0x4) == 4); /* wait until busy */
        sciSendByte(sci,*text++);      /* send out text   */
    }
}
/* USER CODE END */
