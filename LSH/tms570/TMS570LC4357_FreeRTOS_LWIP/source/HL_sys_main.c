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
#include "os_task.h"

/* Include HET header file - types, definitions and function declarations for system driver */
#include "HL_het.h"
#include "HL_gio.h"
#include "HL_esm.h"

#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

void vTask1(void *pvParameters);
extern void vTask2(void *pvParameters);
extern void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName );
extern void EMAC_LwIP_Main (uint8_t * emacAddress);


/* Define Task Handles */
xTaskHandle xTask1Handle;
xTaskHandle xTask2Handle;

/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
/* USER CODE END */

uint8	emacAddress[6U] = 	{0x00U, 0x08U, 0xeeU, 0x03U, 0xa6U, 0x6cU};
uint32 	emacPhyAddress	=	1U;

int main(void)
{
/* USER CODE BEGIN (3) */
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

    //EMAC_LwIP_Main(emacAddress);

    /* Create Task 1 */
    //    if (xTaskCreate(vTask1,"Task1", configMINIMAL_STACK_SIZE, NULL, 1, &xTask1Handle) != pdTRUE)
    if (xTaskCreate(vTask1,"Task1", configMINIMAL_STACK_SIZE, NULL,  ((configMAX_PRIORITIES-1)|portPRIVILEGE_BIT), &xTask1Handle) != pdTRUE)
    {
        /* Task could not be created */
        while(1);
    }

    /* Create Task 2 */
    if (xTaskCreate(vTask2,"Task2", configMINIMAL_STACK_SIZE, NULL, 1, &xTask2Handle) != pdTRUE)
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
    EMAC_LwIP_Main (emacAddress);
#if 1
    for(;;){
        /* Taggle GIOB[6] with timer tick */
        gioSetBit(gioPORTB, 6, gioGetBit(gioPORTB, 6) ^ 1);

        /* Taggle HET[1] with timer tick */
        gioSetBit(hetPORT1, 17, gioGetBit(hetPORT1, 17) ^ 1);  //LED on HDK, top left
        vTaskDelay(300);
    }
#endif
}

void vTask2(void *pvParameters)
{
    for(;;)
    {
        /* Taggle GIOB[7] with timer tick */
        gioSetBit(gioPORTB, 7, gioGetBit(gioPORTB, 7) ^ 1);

        gioSetBit(hetPORT1, 18, gioGetBit(hetPORT1, 18) ^ 1);  //LED on HDK, bottom
        vTaskDelay(500);
    }
}

/* USER CODE END */
