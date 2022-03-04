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
#include "HL_adc.h"

#include <string.h>
#include <stdio.h>

//가독성을 위해 HL_sci.h에 정의된 sciREG1를 UART로 리맵핑
#define UART sciREG1

void sci_display_text(sciBASE_t *sci, uint8 *text, uint32 length);
void sci_display_data(sciBASE_t *sci, uint8 *text, uint32 length);
void wait(uint32 time);
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
char txt_buf[256] = {0};
unsigned int buf_len;

uint8 channel_id_msg[] = {'c', 'h', 'a', 'n', 'n', 'e', 'l', ' ', 'i', 'd', '=', '0', 'x', '\0'};
uint8 channel_data_msg[] = {'v', 'a', 'l', 'u', 'e', '=', '0', 'x','\0'};

adcData_t adc_data;
/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
    uint32 id = 0;
    uint16 value = 0;

    sciInit();
    sprintf(txt_buf, "SCI Init Success!\n\r\0");
    buf_len = strlen(txt_buf);
    sci_display_text(UART, (uint8 *)txt_buf, buf_len);

    gioInit();
    gioSetDirection(gioPORTB, 1);

    sprintf(txt_buf, "GIO Init Success!\n\r\0");
    buf_len = strlen(txt_buf);
    sci_display_text(UART, (uint8 *)txt_buf, buf_len);

    adcInit();
    adcStartConversion(adcREG1, adcGROUP1);

    sprintf(txt_buf, "ADC Init Success!\n\r\0");
    buf_len = strlen(txt_buf);
    sci_display_text(UART, (uint8 *)txt_buf, buf_len);

    while(1)
    {
        //adcStart
        gioSetBit(gioPORTB, 0, 1);

        while((adcIsConversionComplete(adcREG1, adcGROUP1)) == 0);
        adcGetData(adcREG1, adcGROUP1, &adc_data);

        id = adc_data.id;
        value = adc_data.value;

        //adcEnd
        gioSetBit(gioPORTB, 0, 0);

        //ADC ID display
        sprintf(txt_buf, "%s%d\n\r\0", channel_id_msg, id);
        buf_len = strlen(txt_buf);
        sci_display_text(UART, (uint8 *)txt_buf, buf_len);

        //sci_display_data(UART, (uint8 *)&id, 4);

        //ADC result display
        sprintf(txt_buf, "%s%d\n\r\0", channel_data_msg, value);
        buf_len = strlen(txt_buf);
        sci_display_text(UART, (uint8 *)txt_buf, buf_len);

        //sci_display_data(UART, (uint8 *)&value, 4);

        wait(1000000);
    }
/* USER CODE END */

    return 0;
}

/* USER CODE BEGIN (4) */
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
