/*
 * pmic_mp5475.h
 *
 */

#ifndef IC_PMIC_MP5475_H_
#define IC_PMIC_MP5475_H_
#include "stm32f4xx_hal.h"
#include <stdint.h>

/* 고장 우선순위 OV > OC > UV */
typedef enum {
    FAULT_TYPE_NONE = 0,    // 정상
	FAULT_TYPE_OV   = 1,    // OverVoltage
    FAULT_TYPE_OC   = 2,    // OverCurrent
    FAULT_TYPE_UV   = 3,    // UnderVoltage
} FaultType_t;

HAL_StatusTypeDef PMIC_Init(I2C_HandleTypeDef *hi2c);

FaultType_t PMIC_Check_Faults(void);

#endif /* IC_PMIC_MP5475_H_ */
