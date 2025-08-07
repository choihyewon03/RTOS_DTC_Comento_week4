/*
 * eeprom_25lc256.h
 *
 */

#ifndef IC_EEPROM_25LC256_H_
#define IC_EEPROM_25LC256_H_

#include "stm32f4xx_hal.h"

typedef struct {
    uint32_t dtc_code;
    float    snapshot_voltage;
} DTC_Record_t;

void EEPROM_Init(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef EEPROM_Write_Record(DTC_Record_t record);
HAL_StatusTypeDef EEPROM_Read_Record(DTC_Record_t *record);
HAL_StatusTypeDef EEPROM_Clear_Record(void);

#endif /* IC_EEPROM_25LC256_H_ */
