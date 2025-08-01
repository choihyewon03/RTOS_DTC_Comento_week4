/*
 * pmic_mp5475.c
 *
 */

#include "pmic_mp5475.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>

#define PMIC_SLAVE_ADDR      (0x68 << 1)
#define PMIC_REG_STATUS      0x0E
#define PMIC_REG_STATUS2     0x0F

// status register 1 (UV/OC)
typedef union {
    uint8_t rawData;
    struct {
        uint8_t                 : 4;
        uint8_t buck1_oc_flg    : 1;
        uint8_t buck1_uv_flg    : 1;
        uint8_t                 : 2;
    } fields;
} PmicStatusReg1_t;

// status register 2 (OV)
typedef union {
    uint8_t rawData;
    struct {
        uint8_t buck1_ov_flg    : 1;
        uint8_t                 : 7;
    } fields;
} PmicStatusReg2_t;

static I2C_HandleTypeDef *g_hi2c;

HAL_StatusTypeDef PMIC_Init(I2C_HandleTypeDef *hi2c) {
    g_hi2c = hi2c;
    return HAL_OK;
}

FaultType_t PMIC_Check_Faults(void) {
    PmicStatusReg1_t statusReg1; // UV/OC check
    PmicStatusReg2_t statusReg2; // OV check

    // [고장 우선순위 1순위] OV check
    if (HAL_I2C_Mem_Read(g_hi2c, PMIC_SLAVE_ADDR, PMIC_REG_STATUS2,
    		I2C_MEMADD_SIZE_8BIT, &statusReg2.rawData, 1, 10) == HAL_OK) {
        if (statusReg2.fields.buck1_ov_flg) {
            return FAULT_TYPE_OV;
        }
    }

    // [고장 우선순위 2, 3순위] UV/OC check
    if (HAL_I2C_Mem_Read(g_hi2c, PMIC_SLAVE_ADDR, PMIC_REG_STATUS,
    		I2C_MEMADD_SIZE_8BIT, &statusReg1.rawData, 1, 10) == HAL_OK) {
        if (statusReg1.fields.buck1_oc_flg) {
            return FAULT_TYPE_OC;
        }
        if (statusReg1.fields.buck1_uv_flg) {
            return FAULT_TYPE_UV;
        }
    }

    return FAULT_TYPE_NONE;
}

