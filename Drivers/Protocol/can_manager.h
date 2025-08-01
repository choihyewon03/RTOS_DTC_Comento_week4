/*
 * can_manager.h
 *
 */

#ifndef PROTOCOL_CAN_MANAGER_H_
#define PROTOCOL_CAN_MANAGER_H_

#include "stm32f4xx_hal.h"

// CAN 메시지 ID 정의
#define CAN_ID_DTC_BROADCAST    0x18A // 고장 발생 시 브로드캐스트용 ID
#define CAN_ID_DIAG_REQUEST     0x7A0 // 진단기 요청 ID
#define CAN_ID_DIAG_RESPONSE    0x7A8 // 진단기 응답 ID

void CAN_Manager_Init(CAN_HandleTypeDef *hcan);
HAL_StatusTypeDef CAN_Transmit_DTC(uint32_t dtcCode);
HAL_StatusTypeDef CAN_Transmit_Diag_Response(uint8_t *data, uint8_t length); // 진단 응답 함수

#endif /* PROTOCOL_CAN_MANAGER_H_ */
