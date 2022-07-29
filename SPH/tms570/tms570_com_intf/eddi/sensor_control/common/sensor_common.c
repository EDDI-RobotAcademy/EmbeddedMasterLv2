/*
 * sensor_common.c
 *
 *  Created on: 2022. 7. 30.
 *      Author: son
 */
#include "sensor_common.h"

extern spiDAT1_t data_cfg;

void activate_spi_bus(SPIDATAFMT_t ftm, uint8_t active_cs)
{
    data_cfg.CS_HOLD = FALSE;
    data_cfg.WDEL = TRUE;
    //데이터 fifo를 어느것으로 사용할지 설정
    data_cfg.DFSEL = ftm;   //8-bit word length
    data_cfg.CSNR = active_cs; /* CS0*/
}



