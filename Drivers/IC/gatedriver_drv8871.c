/*
 * gatedriver_drv8871.c
 *
 *
 */

#include "gatedriver_drv8871.h"

static SPI_HandleTypeDef *g_hspi;

void GateDriver_Init(SPI_HandleTypeDef *hspi) {
    g_hspi = hspi;
    GateDriver_DisableMotor();
}

void GateDriver_EnableMotor(void) {
    uint8_t enable_cmd = 0xAA; // 모터 활성화
    // HAL_SPI_Transmit(g_hspi, &enable_cmd, 1, 100);
}

void GateDriver_DisableMotor(void) {
    uint8_t disable_cmd = 0x55; // 모터 비활성화
    // HAL_SPI_Transmit(g_hspi, &disable_cmd, 1, 100);
}
