/*
 * gatedriver_drv8871.h
 *
 *
 */

#ifndef IC_GATEDRIVER_DRV8871_H_
#define IC_GATEDRIVER_DRV8871_H_

#include "stm32f4xx_hal.h"

void GateDriver_Init(SPI_HandleTypeDef *hspi);
void GateDriver_EnableMotor(void);
void GateDriver_DisableMotor(void);

#endif /* IC_GATEDRIVER_DRV8871_H_ */
