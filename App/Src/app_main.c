/*
 * app_main.c
 *
 */
#include "app_main.h"
#include "cmsis_os2.h"
#include "pmic_mp5475.h"
#include "eeprom_25lc256.h"
#include "can_manager.h"
#include "gatedriver_drv8871.h"
#include "main.h"
#include <string.h>

// 외부 핸들러 참조
extern I2C_HandleTypeDef hi2c1;
extern SPI_HandleTypeDef hspi1;
extern CAN_HandleTypeDef hcan1;
extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart4;

// RTOS 객체
osMessageQueueId_t canRxQueueHandle;
static osMessageQueueId_t dtcStorageQueueHandle;

// Task 함수 프로토타입
void Task_1ms_CriticalOps(void *argument);
void Task_5ms_Monitoring(void *argument);
void Process_CAN_Command(uint8_t *data);

// DTC 코드 정의
static const uint32_t DTC_BRAKE_VOLTAGE_HIGH = 0xC1010100;
static const uint32_t DTC_BRAKE_VOLTAGE_LOW  = 0xC1010200;
static const uint32_t DTC_PMIC_OV            = 0xC0033333;
static const uint32_t DTC_PMIC_OC            = 0xC0022222;
static const uint32_t DTC_PMIC_UV            = 0xC0011111;

// ADC 전압 스펙 정의
static const float V_REF                   = 3.3f;
static const float ADC_RESOLUTION          = 4095.0f;
static const float VOLTAGE_SPEC_HIGH       = 2.5f;
static const float VOLTAGE_SPEC_LOW        = 1.5f;

// Snapshot -> ADC 전압을 저장할 전역 변수
static volatile float g_lastAdcVoltage = 0.0f;

void App_Main(void) {

    PMIC_Init(&hi2c1);
    EEPROM_Init(&hspi1);
    GateDriver_Init(&hspi1);
    CAN_Manager_Init(&hcan1);

    canRxQueueHandle = osMessageQueueNew(8, 8, NULL); // CAN 수신 데이터(최대 8바이트)
    dtcStorageQueueHandle = osMessageQueueNew(5, sizeof(uint32_t), NULL); // 저장할 DTC 코드

    osThreadNew(Task_1ms_CriticalOps, NULL, &(osThreadAttr_t){.name = "Task_1ms_Critical", .priority = osPriorityHigh});
    osThreadNew(Task_5ms_Monitoring, NULL, &(osThreadAttr_t){.name = "Task_5ms_Monitor", .priority = osPriorityNormal});
}

/* 1ms 주기: I2C/SPI/CAN 핵심 제어 Task */
void Task_1ms_CriticalOps(void *argument) {
    uint32_t tick;
    FaultType_t pmicFault;
    uint32_t dtcFromAdc;
    uint8_t canRxData[8];

    GateDriver_EnableMotor();

    for (;;) {
        tick = osKernelGetTickCount();

        // 1. PMIC Fault 감지 및 처리
        pmicFault = PMIC_Check_Faults();
        if (pmicFault != FAULT_TYPE_NONE) {

        	if (pmicFault == FAULT_TYPE_OV || pmicFault == FAULT_TYPE_OC) {
        		GateDriver_DisableMotor();
        	}

            uint32_t dtcToStore = 0;
            if(pmicFault == FAULT_TYPE_OV) dtcToStore = DTC_PMIC_OV;
            else if(pmicFault == FAULT_TYPE_OC) dtcToStore = DTC_PMIC_OC;
            else if(pmicFault == FAULT_TYPE_UV) dtcToStore = DTC_PMIC_UV;

            // Snapshot 데이터를 포함한 레코드 생성 및 저장
            DTC_Record_t record = {0};
            record.dtc_code = dtcToStore;
            record.snapshot_voltage = g_lastAdcVoltage;
            EEPROM_Write_Record(record);
            CAN_Transmit_DTC_Broadcast(record.dtc_code);
        } else {
        	GateDriver_EnableMotor();
        }

        // 2. 5ms Task로부터 온 ADC 관련 DTC 저장 요청 처리
        if (osMessageQueueGet(dtcStorageQueueHandle, &dtcFromAdc, NULL, 0U) == osOK) {
        	DTC_Record_t record = {0};
        	record.dtc_code = dtcFromAdc;
        	record.snapshot_voltage = g_lastAdcVoltage;
        	EEPROM_Write_Record(record);
            CAN_Transmit_DTC_Broadcast(record.dtc_code);
        }

        // 3. CAN 수신 명령 처리
        if (osMessageQueueGet(canRxQueueHandle, &canRxData, NULL, 0U) == osOK) {
            Process_CAN_Command(canRxData);
        }

        osDelayUntil(tick + 1);
    }
}

/* 5ms 주기: ADC 모니터링 및 UART 로깅 Task */
void Task_5ms_Monitoring(void *argument) {
    uint32_t tick;
    uint32_t adcValue;
    float voltage;
    uint32_t newDtc = 0;

    for (;;) {
        tick = osKernelGetTickCount();

        // 1. ADC 전압 모니터링
        HAL_ADC_Start(&hadc1);
        if (HAL_ADC_PollForConversion(&hadc1, 1) == HAL_OK) {
            adcValue = HAL_ADC_GetValue(&hadc1);
            voltage = (adcValue / ADC_RESOLUTION) * V_REF;
            g_lastAdcVoltage = voltage; // voltage update for Snapshot

            if (voltage > VOLTAGE_SPEC_HIGH) {
                newDtc = DTC_BRAKE_VOLTAGE_HIGH;
            } else if (voltage < VOLTAGE_SPEC_LOW) {
                newDtc = DTC_BRAKE_VOLTAGE_LOW;
            }

            if (newDtc != 0) {
                // 1ms Task에 DTC 저장을 요청
                osMessageQueuePut(dtcStorageQueueHandle, &newDtc, 0U, 0U);
            }
        }
        HAL_ADC_Stop(&hadc1);

        // 2. UART로 ECU 정상 동작 메시지 송출
        HAL_UART_Transmit(&huart4, (uint8_t*)"ECU Status: OK\r\n", 18, 10);

        osDelayUntil(tick + 5);
    }
}

void Process_CAN_Command(uint8_t *data) {
    uint8_t sid = data[0]; // UDS Service ID (SID)
    uint8_t responseData[8] = {0};

    switch(sid) {
            case UDS_SID_READ_DTC: // 0x19
                {
                    DTC_Record_t record = {0};
                    EEPROM_Read_Record(&record);

                    // 응답 데이터 구성
                    // CAN 데이터(8바이트) = SID(1바이트) + Snapshot 데이터(7바이트)
                    responseData[0] = sid;
                    memcpy(&responseData[1], &record, 7); // 구조체를 그대로 복사
                    CAN_Transmit_Diag_Response(responseData, 8);
                }
                break;

            case UDS_SID_CLEAR_DTC: // 0x14
                EEPROM_Clear_Record();
                responseData[0] = sid;
                responseData[1] = 0xAA; // 성공 응답 코드
                CAN_Transmit_Diag_Response(responseData, 2);
                break;
        }
}


