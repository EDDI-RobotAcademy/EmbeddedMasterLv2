/*
 * gy_50_rw.c
 *
 *  Created on: 2022. 8. 2.
 *      Author: son
 */
#include "HL_system.h"

#include "gy_50_rw.h"

spiDAT1_t dataconfig1_t;

void enable_spi_bus(boolean cs_hold, SPIDATAFMT_t fmt, uint8_t cs)
{
    dataconfig1_t.CS_HOLD = cs_hold;
    dataconfig1_t.WDEL = TRUE;
    dataconfig1_t.DFSEL = fmt;   //8-bit word length
    dataconfig1_t.CSNR = cs; /* CS0*/
}

uint32_t write_gy_50(uint16_t mrw, uint16_t reg, uint16_t data)
{
    uint16_t txdata[2];
    uint32_t spiErrorFlag;
    uint16_t blocksize;

    reg |= WRITE << 7;
    reg |= mrw << 6;
    txdata[0] = reg << BE;
    txdata[1] = data;

    blocksize = 2;

    spiErrorFlag = spiTransmitData(spiREG3, &dataconfig1_t, blocksize, &txdata[0]);

    return spiErrorFlag;
}

uint8_t read_gy_50(uint16_t mrw, uint16_t reg)
{
    uint16_t txdata[1];
    uint16_t blocksize;

    reg |= READ << 7;
    reg |= mrw << 6;
    txdata[0] = reg << BE;

    blocksize = 1;

    spiTransmitData(spiREG3, &dataconfig1_t, blocksize, &txdata[0]);

    return spiREG3->BUF;
}


