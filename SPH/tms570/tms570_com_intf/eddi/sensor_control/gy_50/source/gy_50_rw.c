/*
 * gy_50_rw.c
 *
 *  Created on: 2022. 7. 28.
 *      Author: son
 */
#include <stdlib.h>

#include "gy_50_rw.h"
#include "gy_50_en_intf.h"

#define READ 1
#define WRITE 0

//uint32_t gy_50_write(spiDAT1_t *data_cfg, uint16_t address, uint16_t *data, uint16_t block_size)
//{
//    uint16_t *txdata;
//    uint32_t spiErrorFlag = 0;
//
//    txdata = (uint16_t *)malloc(sizeof(uint16_t)*block_size);
//    address = WRITE << 8;
//    txdata[0] = address;
//    spiErrorFlag = spiTransmitData(spiREG3, data_cfg, *block_size, txdata);
//
//    free(txdata);
//
//    return spiErrorFlag;
//}

uint8_t gy_50_read(spiDAT1_t *data_cfg, uint16_t address, uint16_t block_size)
{
    uint16_t *rxdata;

    rxdata = malloc(sizeof(uint16_t) * block_size);
    address |= READ << 7;
    rxdata[0] = address << 8;

    spiTransmitData(spiREG3, data_cfg, block_size, &rxdata[0]);

    free(rxdata);
    return spiREG3->BUF;
}



