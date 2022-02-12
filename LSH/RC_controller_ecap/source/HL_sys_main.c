
#include "HL_sys_common.h"
#include "stdio.h"
#include "string.h"
#include "HL_sci.h"
#include "HL_system.h"
#include "HL_ecap.h"
#include "HL_etpwm.h"

#define UART sciREG1
char txt_buf[256] = { 0 };
unsigned int buf_len;

void sciDisplayText(sciBASE_t *sci, uint8 *text, uint32 len)
{
    while (len--)
    {
        while ((UART->FLR & 0x4) == 4)
            ;
        sciSendByte(UART, *text++);
    }
}

int main(void)
{
    sciInit();
    etpwmInit();
    etpwmStartTBCLK();
    ecapInit();
    _enable_interrupt_();

    ecapStartCounter(ecapREG1);
    ecapStartCounter(ecapREG2);
    ecapStartCounter(ecapREG3);
    ecapStartCounter(ecapREG4);
    ecapEnableCapture(ecapREG1);
    ecapEnableCapture(ecapREG2);
    ecapEnableCapture(ecapREG3);
    ecapEnableCapture(ecapREG4);


    while (1)
        ;

}

void ecapNotification(ecapBASE_t *ecap, uint16 flags)
{


    uint32 cap[8];
    float64 elev,rudd,ail,thro;
    int i;

    cap[0] = ecapGetCAP1(ecapREG1);
    cap[1] = ecapGetCAP2(ecapREG1);
    cap[2] = ecapGetCAP1(ecapREG2);
    cap[3] = ecapGetCAP2(ecapREG2);
    cap[4] = ecapGetCAP1(ecapREG3);
    cap[5] = ecapGetCAP2(ecapREG3);
    cap[6] = ecapGetCAP1(ecapREG4);
    cap[7] = ecapGetCAP2(ecapREG4);

    ail = (cap[3] - cap[2]) * 1000 / VCLK3_FREQ / 1000; // ecap2  channel 1
    elev = (cap[1] - cap[0]) * 1000 / VCLK3_FREQ /1000;  // ecap1  channel 2
    thro = (cap[5] - cap[4]) * 1000 / VCLK3_FREQ / 1000; // ecap3   channel 3
    rudd = (cap[7] - cap[6]) * 1000 / VCLK3_FREQ / 1000; //ecap4     channel 4

    sprintf(txt_buf, "elev = %f ms\t rudd = %f ms\t ail = %f ms\t thro = %f ms\n\r", elev,rudd,ail,thro);
    buf_len = strlen(txt_buf);
    sciDisplayText(UART, (uint8 *) txt_buf, buf_len);

    for(i=0;i<10000;i++);
}
