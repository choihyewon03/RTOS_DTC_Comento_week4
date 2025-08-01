/*
 * app_main.h
 *
 */

#ifndef INC_APP_MAIN_H_
#define INC_APP_MAIN_H_
#include "cmsis_os2.h"

extern osMessageQueueId_t canRxQueueHandle;
void App_Main(void);

#endif /* INC_APP_MAIN_H_ */
