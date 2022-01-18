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
#include "HL_gio.h"
#include "HL_rti.h"
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

int main(void)
{
/* USER CODE BEGIN (3) */
    //peri 설정 초기화
    gioInit();
    rtiInit();

    //리얼타임 인터럽트를 누가 처리할지 설정
    //weak reference : 함수 오버로딩
    //RTI compare0에서 발생하는 인터럽트 허용
    rtiEnableNotification(rtiREG1, rtiNOTIFICATION_COMPARE0);

    //RTI compare1추가
    rtiEnableNotification(rtiREG1, rtiNOTIFICATION_COMPARE1);

    //IRQ 인터럽트 활성화
    //함수 _의 의미 : system(H/W)과 밀접한 함수라는 의미(2개 이상 들어가있으면 중요한 함수로 커스터마이징 금지)
    //CPSIE I를 통해 CPSR의 I 비트인 IRQ를 활성화 한다.
    //_disable_()의 경우 CPSID I를 통해 IRQ를 비활성화 한다.
    _enable_IRQ_interrupt_();

    //리얼타임 인터럽트 카운터 시작 : rtos 스케줄링시 타이밍 조정하는 기능을 할 때도 rti의 카운터가 필요
    //실제로는 RTI counter block0을 시작하여 카운팅을 진행
    rtiStartCounter(rtiREG1, rtiCOUNTER_BLOCK0);
    //RTI counter block1을 시작하여 카운팅을 진행
    rtiStartCounter(rtiREG1, rtiCOUNTER_BLOCK1);
/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
//이 부분은 RTI Interrupt Handler로 카운터가 차면 구동됨
void rtiNotification(rtiBASE_t *rtiREG, uint32_t notification)
{
    switch(notification)
    {
    case rtiNOTIFICATION_COMPARE0:
        gioToggleBit(gioPORTA, 4);
        break;
    case rtiNOTIFICATION_COMPARE1:
        gioToggleBit(gioPORTA, 5);
        break;
    }
#if 0
    //GPIO PORTA의 4번을 토글
    gioToggleBit(gioPORTA, 4);
#endif
}
/* USER CODE END */
