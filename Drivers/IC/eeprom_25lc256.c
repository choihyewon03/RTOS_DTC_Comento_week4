/*
 * eeprom_25lc256.c
 *
 */
#include "eeprom_25lc256.h"

#define EEPROM_CMD_READ     0x03
#define EEPROM_CMD_WRITE    0x02
#define EEPROM_CMD_WREN     0x06

#define EEPROM_DTC_ADDRESS  0x0100
#define EEPROM_CS_PORT      GPIOB
#define EEPROM_CS_PIN       GPIO_PIN_6

static SPI_HandleTypeDef *g_hspi;

static inline void EEPROM_CS_Select() { HAL_GPIO_WritePin(EEPROM_CS_PORT, EEPROM_CS_PIN, GPIO_PIN_RESET); }
static inline void EEPROM_CS_Deselect() { HAL_GPIO_WritePin(EEPROM_CS_PORT, EEPROM_CS_PIN, GPIO_PIN_SET); }

void EEPROM_Init(SPI_HandleTypeDef *hspi) {
    g_hspi = hspi;
    EEPROM_CS_Deselect();
}

HAL_StatusTypeDef EEPROM_Write_Record(DTC_Record_t record) {
    uint8_t wren_cmd = EEPROM_CMD_WREN;
    uint8_t tx_header[3];

    EEPROM_CS_Select();
    HAL_SPI_Transmit(g_hspi, &wren_cmd, 1, 100);
    EEPROM_CS_Deselect();
    HAL_Delay(5);

    tx_header[0] = EEPROM_CMD_WRITE;
    tx_header[1] = (EEPROM_DTC_ADDRESS >> 8) & 0xFF;
    tx_header[2] = EEPROM_DTC_ADDRESS & 0xFF;

    EEPROM_CS_Select();
    HAL_SPI_Transmit(g_hspi, tx_header, 3, 100);
    HAL_SPI_Transmit(g_hspi, (uint8_t*)&record, sizeof(uint32_t), 100);
    EEPROM_CS_Deselect();
    HAL_Delay(5);

    return HAL_OK;
}

HAL_StatusTypeDef EEPROM_Read_Record(DTC_Record_t *record) {
    uint8_t tx_header[3];

    tx_header[0] = EEPROM_CMD_READ;
    tx_header[1] = (EEPROM_DTC_ADDRESS >> 8) & 0xFF;
    tx_header[2] = EEPROM_DTC_ADDRESS & 0xFF;

    EEPROM_CS_Select();
    HAL_SPI_Transmit(g_hspi, tx_header, 3, 100);
    HAL_SPI_Receive(g_hspi, (uint8_t*)record, sizeof(uint32_t), 100);
    EEPROM_CS_Deselect();

    return HAL_OK;
}

HAL_StatusTypeDef EEPROM_Clear_Record(void) {
	DTC_Record_t empty_record = {0};
    return EEPROM_Write_Record(empty_record);
}



