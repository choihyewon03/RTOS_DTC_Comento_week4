/*
 * can_manager.c
 *
 */
#include "can_manager.h"

#define CAN_STB_PORT    GPIOC
#define CAN_STB_PIN     GPIO_PIN_13

static CAN_HandleTypeDef *g_hcan;

void CAN_Manager_Init(CAN_HandleTypeDef *hcan) {
    g_hcan = hcan;
    HAL_GPIO_WritePin(CAN_STB_PORT, CAN_STB_PIN, GPIO_PIN_RESET);
    HAL_CAN_Start(g_hcan);
    HAL_CAN_ActivateNotification(g_hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
}

HAL_StatusTypeDef CAN_Transmit_DTC_Broadcast(uint32_t dtcCode) {
    CAN_TxHeaderTypeDef txHeader;
    uint8_t txData[4];
    uint32_t txMailbox;

    txHeader.StdId = CAN_ID_DTC_BROADCAST;;
    txHeader.IDE = CAN_ID_STD;
    txHeader.RTR = CAN_RTR_DATA;
    txHeader.DLC = 4;

    txData[0] = (dtcCode >> 24) & 0xFF;
    txData[1] = (dtcCode >> 16) & 0xFF;
    txData[2] = (dtcCode >> 8) & 0xFF;
    txData[3] = dtcCode & 0xFF;

    if (HAL_CAN_GetTxMailboxesFreeLevel(g_hcan) == 0) return HAL_BUSY;
    return HAL_CAN_AddTxMessage(g_hcan, &txHeader, txData, &txMailbox);
}

HAL_StatusTypeDef CAN_Transmit_Diag_Response(uint8_t *data, uint8_t length) {
    CAN_TxHeaderTypeDef txHeader;
    uint32_t txMailbox;

    txHeader.StdId = CAN_ID_DIAG_RESPONSE;
    txHeader.IDE = CAN_ID_STD;
    txHeader.RTR = CAN_RTR_DATA;
    txHeader.DLC = length;

    if (HAL_CAN_GetTxMailboxesFreeLevel(g_hcan) == 0) return HAL_BUSY;
    return HAL_CAN_AddTxMessage(g_hcan, &txHeader, data, &txMailbox);

}
