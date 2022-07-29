/*
 * sensor_io_ctrl.c
 *
 *  Created on: 2022. 7. 26.
 *      Author: son
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
    //데이터 fifo를 어느것으로 사용할지 설정
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
    uint32_t blockSize;
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

/* USER CODE BEGIN (4) */
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

void eeprom_write_enable (void)
{
    uint16_t eeprom_wren = EEPROM_WREN;

    spi_conf_set();
    spiTransmitData(spiREG1, &conf, 1, &eeprom_wren);
}

void eeprom_write_byte (uint8_t address, uint8_t data)
{
#if 1
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



