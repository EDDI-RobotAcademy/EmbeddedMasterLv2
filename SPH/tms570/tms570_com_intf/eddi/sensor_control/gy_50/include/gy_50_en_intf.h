/*
 * gy_50_en_intf.h
 *
 *  Created on: 2022. 7. 29.
 *      Author: son
 */

#ifndef EDDI_SENSOR_CONTROL_GY_50_INCLUDE_GY_50_EN_INTF_H_
#define EDDI_SENSOR_CONTROL_GY_50_INCLUDE_GY_50_EN_INTF_H_

#include "HL_sys_common.h"
#include "HL_spi.h"

extern void gy_50_en_spi(spiDAT1_t *pdata, SPIDATAFMT_t ftm, uint8_t active_cs);


#endif /* EDDI_SENSOR_CONTROL_GY_50_INCLUDE_GY_50_EN_INTF_H_ */
