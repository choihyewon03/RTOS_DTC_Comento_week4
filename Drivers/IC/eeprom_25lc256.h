/*
 * eeprom_25lc256.h
 *
 */

#ifndef IC_EEPROM_25LC256_H_
#define IC_EEPROM_25LC256_H_

#include "stm32f4xx_hal.h"

void EEPROM_Init(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef EEPROM_Write_DTC(uint32_t dtcCode);
HAL_StatusTypeDef EEPROM_Read_DTC(uint32_t *dtcCode);
HAL_StatusTypeDef EEPROM_Clear_DTC(void);

#endif /* IC_EEPROM_25LC256_H_ */
