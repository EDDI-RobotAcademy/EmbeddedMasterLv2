/*
 * sensor_common.h
 *
 *  Created on: 2022. 7. 30.
 *      Author: son
 */

#ifndef EDDI_SENSOR_CONTROL_COMMON_SENSOR_COMMON_H_
#define EDDI_SENSOR_CONTROL_COMMON_SENSOR_COMMON_H_

#include "HL_sys_common.h"
#include "HL_system.h"
#include "HL_spi.h"

#define DEVICE_GY50 1
#define DEVICE_25LC010A 2

extern void activate_spi_bus(SPIDATAFMT_t ftm, uint8_t active_cs);

#endif /* EDDI_SENSOR_CONTROL_COMMON_SENSOR_COMMON_H_ */
