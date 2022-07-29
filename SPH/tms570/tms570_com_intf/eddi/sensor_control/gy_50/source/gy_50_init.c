/*
 * gy_50_intf.c
 *
 *  Created on: 2022. 7. 27.
 *      Author: son
 */

#include "HL_sys_common.h"
#include "HL_system.h"
#include "HL_spi.h"

#include "gy_50_init.h"

/** @struct g_spiPacket
*   @brief globals
*
*/
static volatile struct g_spiPacket
{
    spiDAT1_t g_spiDataFormat;
    uint32  tx_length;
    uint32  rx_length;
    uint16  * txdata_ptr;
    uint16  * rxdata_ptr;
    SpiDataStatus_t tx_data_status;
    SpiDataStatus_t rx_data_status;
} g_spiPacket_t[5U];

void gy_50_init_spi_intf(void)
{
    /** @b initialize @b SPI3 */

    /** bring SPI out of reset */
    spiREG3->GCR0 = 0U;
    spiREG3->GCR0 = 1U;

    /** SPI3 master mode and clock configuration */
    spiREG3->GCR1 = (spiREG3->GCR1 & 0xFFFFFFFCU) | ((uint32)((uint32)1U << 1U)  /* CLOKMOD */
                  | 1U);  /* MASTER */

    /** SPI3 enable pin configuration */
    spiREG3->INT0 = (spiREG3->INT0 & 0xFEFFFFFFU) | (uint32)((uint32)0U << 24U);  /* ENABLE HIGHZ */

    /** - Delays */
    spiREG3->DELAY = (uint32)((uint32)0U << 24U)  /* C2TDELAY */
                   | (uint32)((uint32)0U << 16U)  /* T2CDELAY */
                   | (uint32)((uint32)0U << 8U)   /* T2EDELAY */
                   | (uint32)((uint32)0U << 0U);  /* C2EDELAY */

    /** - Data Format 0 */
    spiREG3->FMT0 = (uint32)((uint32)0U << 24U)  /* wdelay */
                  | (uint32)((uint32)0U << 23U)  /* parity Polarity */
                  | (uint32)((uint32)0U << 22U)  /* parity enable */
                  | (uint32)((uint32)0U << 21U)  /* wait on enable */
                  | (uint32)((uint32)0U << 20U)  /* shift direction */
                  | (uint32)((uint32)0U << 17U)  /* clock polarity */
                  | (uint32)((uint32)0U << 16U)  /* clock phase */
                  | (uint32)((uint32)74U << 8U) /* baudrate prescale */
                  | (uint32)((uint32)16U << 0U);  /* data word length */

    /** - Data Format 1 */
    spiREG3->FMT1 = (uint32)((uint32)0U << 24U)  /* wdelay */
                  | (uint32)((uint32)0U << 23U)  /* parity Polarity */
                  | (uint32)((uint32)0U << 22U)  /* parity enable */
                  | (uint32)((uint32)0U << 21U)  /* wait on enable */
                  | (uint32)((uint32)0U << 20U)  /* shift direction */
                  | (uint32)((uint32)0U << 17U)  /* clock polarity */
                  | (uint32)((uint32)0U << 16U)  /* clock phase */
                  | (uint32)((uint32)74U << 8U) /* baudrate prescale */
                  | (uint32)((uint32)16U << 0U);  /* data word length */

    /** - Data Format 2 */
    spiREG3->FMT2 = (uint32)((uint32)0U << 24U)  /* wdelay */
                  | (uint32)((uint32)0U << 23U)  /* parity Polarity */
                  | (uint32)((uint32)0U << 22U)  /* parity enable */
                  | (uint32)((uint32)0U << 21U)  /* wait on enable */
                  | (uint32)((uint32)0U << 20U)  /* shift direction */
                  | (uint32)((uint32)0U << 17U)  /* clock polarity */
                  | (uint32)((uint32)0U << 16U)  /* clock phase */
                  | (uint32)((uint32)74U << 8U) /* baudrate prescale */
                  | (uint32)((uint32)16U << 0U);  /* data word length */

    /** - Data Format 3 */
    spiREG3->FMT3 = (uint32)((uint32)0U << 24U)  /* wdelay */
                  | (uint32)((uint32)0U << 23U)  /* parity Polarity */
                  | (uint32)((uint32)0U << 22U)  /* parity enable */
                  | (uint32)((uint32)0U << 21U)  /* wait on enable */
                  | (uint32)((uint32)0U << 20U)  /* shift direction */
                  | (uint32)((uint32)0U << 17U)  /* clock polarity */
                  | (uint32)((uint32)0U << 16U)  /* clock phase */
                  | (uint32)((uint32)74U << 8U) /* baudrate prescale */
                  | (uint32)((uint32)16U << 0U);  /* data word length */

    /** - set interrupt levels */
    spiREG3->LVL = (uint32)((uint32)0U << 9U)  /* TXINT */
                 | (uint32)((uint32)0U << 8U)  /* RXINT */
                 | (uint32)((uint32)0U << 6U)  /* OVRNINT */
                 | (uint32)((uint32)0U << 4U)  /* BITERR */
                 | (uint32)((uint32)0U << 3U)  /* DESYNC */
                 | (uint32)((uint32)0U << 2U)  /* PARERR */
                 | (uint32)((uint32)0U << 1U) /* TIMEOUT */
                 | (uint32)((uint32)0U << 0U);  /* DLENERR */

    /** - clear any pending interrupts */
    spiREG3->FLG |= 0xFFFFU;

    /** - enable interrupts */
    spiREG3->INT0 = (spiREG3->INT0 & 0xFFFF0000U)
                  | (uint32)((uint32)0U << 9U)  /* TXINT */
                  | (uint32)((uint32)0U << 8U)  /* RXINT */
                  | (uint32)((uint32)0U << 6U)  /* OVRNINT */
                  | (uint32)((uint32)0U << 4U)  /* BITERR */
                  | (uint32)((uint32)0U << 3U)  /* DESYNC */
                  | (uint32)((uint32)0U << 2U)  /* PARERR */
                  | (uint32)((uint32)0U << 1U) /* TIMEOUT */
                  | (uint32)((uint32)0U << 0U);  /* DLENERR */

    /** @b initialize @b SPI3 @b Port */

    /** - SPI3 Port output values */
    spiREG3->PC3 =    (uint32)((uint32)1U << 0U)  /* SCS[0] */
                    | (uint32)((uint32)1U << 1U)  /* SCS[1] */
                    | (uint32)((uint32)1U << 2U)  /* SCS[2] */
                    | (uint32)((uint32)1U << 3U)  /* SCS[3] */
                    | (uint32)((uint32)1U << 4U)  /* SCS[4] */
                    | (uint32)((uint32)1U << 5U)  /* SCS[5] */
                    | (uint32)((uint32)0U << 8U)  /* ENA */
                    | (uint32)((uint32)0U << 9U)  /* CLK */
                    | (uint32)((uint32)0U << 10U)  /* SIMO */
                    | (uint32)((uint32)0U << 11U); /* SOMI */

    /** - SPI3 Port direction */
    spiREG3->PC1  =   (uint32)((uint32)1U << 0U)  /* SCS[0] */
                    | (uint32)((uint32)1U << 1U)  /* SCS[1] */
                    | (uint32)((uint32)1U << 2U)  /* SCS[2] */
                    | (uint32)((uint32)1U << 3U)  /* SCS[3] */
                    | (uint32)((uint32)1U << 4U)  /* SCS[4] */
                    | (uint32)((uint32)1U << 5U)  /* SCS[5] */
                    | (uint32)((uint32)0U << 8U)  /* ENA */
                    | (uint32)((uint32)1U << 9U)  /* CLK */
                    | (uint32)((uint32)1U << 10U)  /* SIMO */
                    | (uint32)((uint32)0U << 11U); /* SOMI */

    /** - SPI3 Port open drain enable */
    spiREG3->PC6  =   (uint32)((uint32)0U << 0U)  /* SCS[0] */
                    | (uint32)((uint32)0U << 1U)  /* SCS[1] */
                    | (uint32)((uint32)0U << 2U)  /* SCS[2] */
                    | (uint32)((uint32)0U << 3U)  /* SCS[3] */
                    | (uint32)((uint32)0U << 4U)  /* SCS[4] */
                    | (uint32)((uint32)0U << 5U)  /* SCS[5] */
                    | (uint32)((uint32)0U << 8U)  /* ENA */
                    | (uint32)((uint32)0U << 9U)  /* CLK */
                    | (uint32)((uint32)0U << 10U)  /* SIMO */
                    | (uint32)((uint32)0U << 11U); /* SOMI */

    /** - SPI3 Port pullup / pulldown selection */
    spiREG3->PC8  =   (uint32)((uint32)1U << 0U)  /* SCS[0] */
                    | (uint32)((uint32)1U << 1U)  /* SCS[1] */
                    | (uint32)((uint32)1U << 2U)  /* SCS[2] */
                    | (uint32)((uint32)1U << 3U)  /* SCS[3] */
                    | (uint32)((uint32)1U << 4U)  /* SCS[4] */
                    | (uint32)((uint32)1U << 5U)  /* SCS[5] */
                    | (uint32)((uint32)1U << 8U)  /* ENA */
                    | (uint32)((uint32)1U << 9U)  /* CLK */
                    | (uint32)((uint32)1U << 10U)  /* SIMO */
                    | (uint32)((uint32)1U << 11U); /* SOMI */

    /** - SPI3 Port pullup / pulldown enable*/
    spiREG3->PC7  =   (uint32)((uint32)0U << 0U)  /* SCS[0] */
                    | (uint32)((uint32)0U << 1U)  /* SCS[1] */
                    | (uint32)((uint32)0U << 2U)  /* SCS[2] */
                    | (uint32)((uint32)0U << 3U)  /* SCS[3] */
                    | (uint32)((uint32)0U << 4U)  /* SCS[4] */
                    | (uint32)((uint32)0U << 5U)  /* SCS[5] */
                    | (uint32)((uint32)0U << 8U)  /* ENA */
                    | (uint32)((uint32)0U << 9U)  /* CLK */
                    | (uint32)((uint32)0U << 10U)  /* SIMO */
                    | (uint32)((uint32)0U << 11U); /* SOMI */

    /* SPI3 set all pins to functional */
    spiREG3->PC0  =   (uint32)((uint32)1U << 0U)  /* SCS[0] */
                    | (uint32)((uint32)1U << 1U)  /* SCS[1] */
                    | (uint32)((uint32)1U << 2U)  /* SCS[2] */
                    | (uint32)((uint32)1U << 3U)  /* SCS[3] */
                    | (uint32)((uint32)1U << 4U)  /* SCS[4] */
                    | (uint32)((uint32)1U << 5U)  /* SCS[5] */
                    | (uint32)((uint32)1U << 8U)  /* ENA */
                    | (uint32)((uint32)1U << 9U)  /* CLK */
                    | (uint32)((uint32)1U << 10U)  /* SIMO */
                    | (uint32)((uint32)1U << 11U); /* SOMI */

    /** - Initialize TX and RX data buffer Status */
    g_spiPacket_t[2U].tx_data_status  = SPI_READY;
    g_spiPacket_t[2U].rx_data_status  = SPI_READY;

    /** - Finally start SPI3 */
    spiREG3->GCR1 = (spiREG3->GCR1 & 0xFEFFFFFFU) | 0x01000000U;
}




