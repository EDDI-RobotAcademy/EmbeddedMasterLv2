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
#include <stdio.h>
#include <string.h>

#include "HL_system.h"
#include "HL_rti.h"
#include "HL_sci.h"
#include "HL_i2c.h"

#define UART sciREG1
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
#define MPU6050_ADDR 0x68

void sci_display_text(sciBASE_t *sci, uint8 *text, uint32 length);
void sci_display_data(sciBASE_t *sci, uint8 *text, uint32 length);
void wait(uint32 time);

void MPU6050_enable(void);
void MPU6050_acc_config(void);

char txt_buf[256] = {0};
unsigned int buf_len;

volatile char g_acc_xyz[6];
volatile int g_acc_flag;
/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
    signed short acc_x, acc_y, acc_z;
    double real_acc_x, real_acc_y, real_acc_z;

    sciInit();
    sprintf(txt_buf, "SCI Init Success!\n\r\0");
    buf_len = strlen(txt_buf);
    sci_display_text(UART, (uint8 *)txt_buf, buf_len);

    rtiInit();
    sprintf(txt_buf, "RTI Init Success!\n\r\0");
    buf_len = strlen(txt_buf);
    sci_display_text(UART, (uint8 *)txt_buf, buf_len);

    /*rti 관련 함수*/
    rtiEnableNotification(rtiREG1, rtiNOTIFICATION_COMPARE2);

    //IRQ 인터럽트 활성화
    //함수 _의 의미 : system(H/W)과 밀접한 함수라는 의미(2개 이상 들어가있으면 중요한 함수로 커스터마이징 금지)
    //CPSIE I를 통해 CPSR의 I 비트인 IRQ를 활성화 한다.
    //_disable_()의 경우 CPSID I를 통해 IRQ를 비활성화 한다.
    _enable_IRQ_interrupt_();

    //리얼타임 인터럽트 카운터 시작 : rtos 스케줄링시 타이밍 조정하는 기능을 할 때도 rti의 카운터가 필요
    //실제로는 RTI counter block1을 시작하여 카운팅을 진행
    rtiStartCounter(rtiREG1, rtiCOUNTER_BLOCK1);

    i2cInit();
    sprintf(txt_buf, "I2C Init Success!\n\r\0");
    buf_len = strlen(txt_buf);
    sci_display_text(UART, (uint8 *)txt_buf, buf_len);

    MPU6050_enable();
    sprintf(txt_buf, "MPU6050 Enable Success!\n\r\0");
    buf_len = strlen(txt_buf);
    sci_display_text(UART, (uint8 *)txt_buf, buf_len);

    wait(200);

    MPU6050_acc_config();
    sprintf(txt_buf, "MPU6050 Accelerometer Config Success!\n\r\0");
    buf_len = strlen(txt_buf);
    sci_display_text(UART, (uint8 *)txt_buf, buf_len);

    wait(200);

    while(1)
    {
        if(g_acc_flag == 1)
        {
            acc_x = acc_y = acc_z = 0;
            real_acc_x = real_acc_y = real_acc_z = 0.0;

            acc_x = g_acc_xyz[0];
            acc_x = acc_x<<8;
            acc_x |= g_acc_xyz[1];

            real_acc_x = ((double)acc_x)/2048.0;

            acc_y = g_acc_xyz[2];
            acc_y = acc_y<<8;
            acc_y |= g_acc_xyz[3];

            real_acc_y = ((double)acc_y)/2048.0;

            acc_z = g_acc_xyz[4];
            acc_z = acc_z<<8;
            acc_z |= g_acc_xyz[5];

            real_acc_z = ((double)acc_z)/2048.0;

            sprintf(txt_buf, "acc_x = %3.5lf\tacc_y = %3.5lf\tacc_z = %3.5lf\n\r\0",
                   real_acc_x, real_acc_y, real_acc_z);

            buf_len = strlen(txt_buf);
            sci_display_text(UART, (uint8 *)txt_buf, buf_len);

            g_acc_flag = 0;
        }
    }
/* USER CODE END */
    return 0;
}

/* USER CODE BEGIN (4) */
void rtiNotification(rtiBASE_t *rtiREG, uint32_t notification)
{
    unsigned char slave_addr = 0x3BU;

    i2cSetSlaveAdd(i2cREG2, MPU6050_ADDR);
    i2cSetDirection(i2cREG2, I2C_TRANSMITTER);
    //slave address
    i2cSetCount(i2cREG2, 1);
    i2cSetMode(i2cREG2, I2C_MASTER);
    i2cSetStop(i2cREG2);
    i2cSetStart(i2cREG2);
    i2cSendByte(i2cREG2, slave_addr);

    while(i2cIsBusBusy(i2cREG2) == 1);
    while(i2cIsStopDetected(i2cREG2) == 0);

    i2cClearSCD(i2cREG2);

    i2cSetDirection(i2cREG2, I2C_RECEIVER);
    //slave address
    i2cSetCount(i2cREG2, 6);
    i2cSetMode(i2cREG2, I2C_MASTER);
    i2cSetStart(i2cREG2);

    i2cReceive(i2cREG2, 6, (unsigned char *)g_acc_xyz);
    i2cSetStop(i2cREG2);

    while(i2cIsBusBusy(i2cREG2) == 1);
    while(i2cIsStopDetected(i2cREG2) == 0);

    i2cClearSCD(i2cREG2);

    g_acc_flag = 1;
}
void MPU6050_enable(void)
{
    unsigned int cnt = 2;
    unsigned char data[2] = { 0x00U, 0x00U };
    unsigned char slave_addr = 0x6BU;

    i2cSetSlaveAdd(i2cREG2, MPU6050_ADDR);
    i2cSetDirection(i2cREG2, I2C_TRANSMITTER);
    //slave address + data
    i2cSetCount(i2cREG2, cnt + 1);
    i2cSetMode(i2cREG2, I2C_MASTER);
    i2cSetStop(i2cREG2);
    i2cSetStart(i2cREG2);
    i2cSendByte(i2cREG2, slave_addr);
    i2cSend(i2cREG2, cnt, data);

    while(i2cIsBusBusy(i2cREG2) == 1);
    while(i2cIsStopDetected(i2cREG2) == 0);

    i2cClearSCD(i2cREG2);

    wait(100000);
}
void MPU6050_acc_config(void)
{
    unsigned int cnt = 1;
    unsigned char data[1] = { 0x18U };
    unsigned char slave_addr = 0x1CU;

    i2cSetSlaveAdd(i2cREG2, MPU6050_ADDR);
    i2cSetDirection(i2cREG2, I2C_TRANSMITTER);
    //slave address + data
    i2cSetCount(i2cREG2, cnt + 1);
    i2cSetMode(i2cREG2, I2C_MASTER);
    i2cSetStop(i2cREG2);
    i2cSetStart(i2cREG2);
    i2cSendByte(i2cREG2, slave_addr);
    i2cSend(i2cREG2, cnt, data);

    while(i2cIsBusBusy(i2cREG2) == 1);
    while(i2cIsStopDetected(i2cREG2) == 0);

    i2cClearSCD(i2cREG2);

    wait(100000);
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
