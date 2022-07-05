```c
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
#include "HL_sci.h"
#include "HL_spi.h"
#include "HL_system.h"

#include <string.h>
#include <stdio.h>
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
void sci_display_text (sciBASE_t *sci, uint8 *text, uint32 length);
void wait (uint32 time);

#define UART        sciREG1

void eeprom_change_byte (uint8_t);
void eeprom_send_address(uint8_t);
uint8_t eeprom_read_status(void);
void eeprom_write_enable(void);
uint8_t eeprom_read_byte(uint8_t);
//void eeprom_read_byte(uint8_t);
void eeprom_write_byte(uint8_t, uint8_t);

uint8 eeprom_recv[3] = { 0 };

#if 0
#define EEPROM_READ     0b00000011
#define EEPROM_WRITE    0b00000010
#define EEPROM_WREN     0b00000110
#define EEPROM_RDSR     0b00000101
#else
#define EEPROM_READ     3
#define EEPROM_WRITE    2
#define EEPROM_WREN     6
#define EEPROM_RDSR     5
#endif

#define EEPROM_WRITE_IN_PROGRESS    0

spiDAT1_t conf;

void spi_conf_init(void);

//uint32_t eeprom_wren(uint8_t address, uint8_t data)
uint32_t eeprom_wren()
{
    spiDAT1_t dataconfig1_t;
    uint32_t DataLength, blockSize;
    uint32_t spiErrorFlag;
    uint16_t txData[1];

    //DataLength = 1; //number of 32-bit words
    //blockSize = DataLength * 4 + 4; /* # of bytes*/
    blockSize = 1;

    dataconfig1_t.CS_HOLD = FALSE;
    dataconfig1_t.WDEL = TRUE;
    dataconfig1_t.DFSEL = SPI_FMT_0;   //8-bit word length
    dataconfig1_t.CSNR = 0xFE; /* CS0*/

    txData[0] = EEPROM_WREN;
    //txData[1] = (address & 0xFF00) >> 0x8;
    //txData[2] = (address & 0x00FF) >> 0x0;
    //txData[3] = DataLength;
    //txData[4] = (data >> 24) & 0xFF;
    //txData[5] = (data >> 16) & 0xFF;
    //txData[6] = (data >> 8 ) & 0xFF;
    //txData[7] = (data >> 0 ) & 0xFF;

    spiErrorFlag = spiTransmitData(spiREG3, &dataconfig1_t, blockSize, &txData[0]);
    return spiErrorFlag;
}

uint32_t eeprom_write(uint16_t address, uint16_t data)
{
    spiDAT1_t dataconfig1_t;
    uint32_t DataLength, blockSize;
    uint32_t spiErrorFlag;
    uint16_t txData[3];

    //DataLength = 1; //number of 32-bit words
    //blockSize = DataLength * 4 + 4; /* # of bytes*/
    blockSize = 3;

    dataconfig1_t.CS_HOLD = FALSE;
    dataconfig1_t.WDEL = TRUE;
    dataconfig1_t.DFSEL = SPI_FMT_0;   //8-bit word length
    dataconfig1_t.CSNR = 0xFE; /* CS0*/

    txData[0] = EEPROM_WRITE;
    txData[1] = address;
    txData[2] = data;
    //txData[1] = (address & 0xFF00) >> 0x8;
    //txData[2] = (address & 0x00FF) >> 0x0;
    //txData[3] = DataLength;
    //txData[4] = (data >> 24) & 0xFF;
    //txData[5] = (data >> 16) & 0xFF;
    //txData[6] = (data >> 8 ) & 0xFF;
    //txData[7] = (data >> 0 ) & 0xFF;

    spiErrorFlag = spiTransmitData(spiREG3, &dataconfig1_t, blockSize, &txData[0]);
    return spiErrorFlag;
}

uint8_t eeprom_rdsr(void)
{
    spiDAT1_t dataconfig1_t;
    uint32_t DataLength, blockSize;
    uint32_t spiErrorFlag;
    uint16_t txData[2];
    //uint16_t rxData[1];

    //DataLength = 1; //number of 32-bit words
    //blockSize = DataLength * 4 + 4; /* # of bytes*/
    blockSize = 2;

    dataconfig1_t.CS_HOLD = TRUE;
    dataconfig1_t.WDEL = TRUE;
    dataconfig1_t.DFSEL = SPI_FMT_0;   //8-bit word length
    dataconfig1_t.CSNR = 0xFE; /* CS0*/

    txData[0] = EEPROM_RDSR;
    txData[1] = 0;
    //txData[1] = (address & 0xFF00) >> 0x8;
    //txData[2] = (address & 0x00FF) >> 0x0;
    //txData[3] = DataLength;
    //txData[4] = (data >> 24) & 0xFF;
    //txData[5] = (data >> 16) & 0xFF;
    //txData[6] = (data >> 8 ) & 0xFF;
    //txData[7] = (data >> 0 ) & 0xFF;

    spiErrorFlag = spiTransmitData(spiREG3, &dataconfig1_t, blockSize, &txData[0]);

#if 0
    while (spiREG3->BUF & 0x80000000)
    {
        ;
    }
#endif

    //spiTransmitData(spiREG3, &dataconfig1_t, blockSize, &txData[0]);

    //spiReceiveData(spiREG3, &dataconfig1_t, blockSize, &rxData[0]);

    return spiREG3->BUF;
}

uint8_t eeprom_read(uint16_t address)
{
    spiDAT1_t dataconfig1_t;
    uint32_t DataLength, blockSize;
    uint32_t spiErrorFlag;
    uint16_t txData[2];
    uint16_t rxData[1];

    //DataLength = 1; //number of 32-bit words
    //blockSize = DataLength * 4 + 4; /* # of bytes*/
    blockSize = 3;

    dataconfig1_t.CS_HOLD = TRUE;
    dataconfig1_t.WDEL = TRUE;
    dataconfig1_t.DFSEL = SPI_FMT_0;   //8-bit word length
    dataconfig1_t.CSNR = 0xFE; /* CS0*/

    txData[0] = EEPROM_READ;
    txData[1] = address;
    txData[2] = 0;
    //txData[1] = (address & 0xFF00) >> 0x8;
    //txData[2] = (address & 0x00FF) >> 0x0;
    //txData[3] = DataLength;
    //txData[4] = (data >> 24) & 0xFF;
    //txData[5] = (data >> 16) & 0xFF;
    //txData[6] = (data >> 8 ) & 0xFF;
    //txData[7] = (data >> 0 ) & 0xFF;

    spiErrorFlag = spiTransmitData(spiREG3, &dataconfig1_t, blockSize, &txData[0]);
    //wait(5);

    //spiReceiveData(spiREG3, &dataconfig1_t, 1, &rxData[0]);

    return spiREG3->BUF;
}
/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
    int i, j;
    char buf[128];
    unsigned int buf_len;

    sciInit();

    sprintf(buf, "SCI Init Success!\n\r\0");
    buf_len = strlen(buf);
    sci_display_text(UART, (uint8 *)buf, buf_len);

    spiInit();

    sprintf(buf, "SPI Init Success!\n\r\0");
    buf_len = strlen(buf);
    sci_display_text(UART, (uint8 *)buf, buf_len);

#if 0
    spi_conf_init();

    sprintf(buf, "SPI Config Init Success!\n\r\0");
    buf_len = strlen(buf);
    sci_display_text(UART, (uint8 *)buf, buf_len);
#endif

    for (i = 0; i < 128; i++)
    {
        uint8_t status;
#if 0
        //spiTransmitAndReceiveData()
        eeprom_write_byte(i, i);

        sprintf(buf, "SPI TX Status: %d\n\r\0", SpiTxStatus(spiREG1));
        buf_len = strlen(buf);
        sci_display_text(UART, (uint8 *)buf, buf_len);
#endif

        sprintf(buf, "EEPROM WREN: %d\n\r\0", eeprom_wren());
        buf_len = strlen(buf);
        sci_display_text(UART, (uint8 *)buf, buf_len);

        sprintf(buf, "SPI TX Status: %d\n\r\0", SpiTxStatus(spiREG3));
        buf_len = strlen(buf);
        sci_display_text(UART, (uint8 *)buf, buf_len);

        sprintf(buf, "EEPROM WRITE: %d\n\r\0", eeprom_write(i, i));
        buf_len = strlen(buf);
        sci_display_text(UART, (uint8 *)buf, buf_len);

        while ((status = eeprom_rdsr()) & 1)
        {
            //sprintf(buf, "EEPROM READ Status: %d\n\r\0", status);
            //buf_len = strlen(buf);
            //sci_display_text(UART, (uint8 *)buf, buf_len);
        }

#if 0
        sprintf(buf, "EEPROM Status: %d\n\r\0", eeprom_read_status());
        buf_len = strlen(buf);
        sci_display_text(UART, (uint8 *)buf, buf_len);
#endif
    }

    for (i = 0; i < 128; i++)
    {
        sprintf(buf, "read data = %d\n\r\0", eeprom_read(i));
        buf_len = strlen(buf);
        sci_display_text(UART, (uint8 *)buf, buf_len);

        sprintf(buf, "SPI RX Status: %d\n\r\0", SpiRxStatus(spiREG1));
        buf_len = strlen(buf);
        sci_display_text(UART, (uint8 *)buf, buf_len);
    }

    for (;;)
        ;
/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
void sci_display_text (sciBASE_t *sci, uint8 *text, uint32 length)
{
    while (length--)
    {
        while ((UART->FLR & 0x4) == 4)
            ;

        sciSendByte(UART, *text++);
    }
}

void wait (uint32 time)
{
    int i;

    for (i = 0; i < time; i++)
        ;
}

void spi_conf_set(void)
{
    conf.CS_HOLD    = TRUE;
    conf.WDEL       = TRUE;
    conf.DFSEL      = SPI_FMT_0;
    conf.CSNR       = 0xFE;
}

#if 0
void spi_eeprom_send_command(spiBASE_t *spi, spiDAT1_t *conf, uint8_t command)
{
    volatile uint32 spi_buf;
    uint16 tx_data;
    //uint32 chip_select_hold = (conf->CS_HOLD) ? 0x10000000U : 0U;
    uint32 chip_select_hold;
    uint32 w_delay = (conf->WDEL) ? 0x04000000U : 0U;
    SPIDATAFMT_t data_format = conf->DFSEL;
    uint8 chip_select = conf->CSNR;

    if((spi->FLG & 0x000000FFU) !=0U)
    {
       break;
    }

    chip_select_hold = 0U;

    tx_data = command;

    spi->DAT1 =  ((uint32)data_format << 24U)   |
                 ((uint32)chip_select << 16U)   |
                 (w_delay)                      |
                 (chip_select_hold)             |
                 (uint32)tx_data;

    while((spi->FLG & 0x00000100U) != 0x00000100U)
    {
    }

    spi_buf = spi->BUF;

    return (spi->FLG & 0xFFU);
}
#endif

void eeprom_write_enable (void)
{
    uint16_t eeprom_wren = EEPROM_WREN;

    spi_conf_set();
    spiTransmitData(spiREG1, &conf, 1, &eeprom_wren);
}

void eeprom_write_byte (uint8_t address, uint8_t data)
{
#if 1
    int i;
    char buf[128];
    unsigned int buf_len;
    uint16_t eeprom_write[] = { EEPROM_WRITE, address, data };

    eeprom_write_enable();
    sprintf(buf, "SPI WREN TX Status: %d\n\r\0", SpiTxStatus(spiREG1));
    buf_len = strlen(buf);
    sci_display_text(UART, (uint8 *)buf, buf_len);

    spi_conf_set();
    spiTransmitData(spiREG1, &conf, 3, eeprom_write);
#endif

#if 1
    while (eeprom_read_status() & 1)
    {
        ;
    }
#endif
}

//void eeprom_read_byte (uint8_t address)
uint8_t eeprom_read_byte (uint8_t address)
{
    int i;
    uint8 eeprom_read[] = { EEPROM_READ, address, 0 };
    uint8 eeprom_recv;

    for (i = 0; i < 2; i++)
    {
        spiTransmitData(spiREG1, &conf, 2, eeprom_read[i]);
    }
    spiTransmitAndReceiveData(spiREG1, &conf, 1, eeprom_read[2], eeprom_recv);

    //return spiREG1->BUF;
    //return eeprom_recv[2];
    return eeprom_recv;
}

uint8_t eeprom_read_status (void)
{
    //uint8 eeprom_rdsr = EEPROM_RDSR;
    //uint8 dummy = 0;
    uint16_t eeprom_rdsr[] = { EEPROM_RDSR, 0 };
    uint16_t status[2] = { 0 };

    //spiTransmitData(spiREG1, &conf, 1, &eeprom_rdsr);
    //spiTransmitAndReceiveData(spiREG1, &conf, 1, &dummy, &status);

#if 1
    spiTransmitAndReceiveData(spiREG1, &conf, 2, eeprom_rdsr, status);
#else
    spiTransmitData(spiREG1, &conf, 2, eeprom_rdsr);
    spiReceiveData(spiREG1, &conf, 1, status);
#endif

    return status[1];
}
/* USER CODE END */

```
