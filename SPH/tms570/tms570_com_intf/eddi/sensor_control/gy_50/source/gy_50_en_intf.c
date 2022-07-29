/*
 * gy_50_en_intf.c
 *
 *  Created on: 2022. 7. 29.
 *      Author: son
 */
#include "gy_50_en_intf.h"

void gy_50_en_spi(spiDAT1_t *pdata, SPIDATAFMT_t ftm, uint8_t active_cs)
{
    pdata->CS_HOLD = FALSE;
    pdata->WDEL = TRUE;
    //데이터 fifo를 어느것으로 사용할지 설정
    pdata->DFSEL = ftm;   //8-bit word length
    pdata->CSNR = active_cs; /* CS0*/
}


