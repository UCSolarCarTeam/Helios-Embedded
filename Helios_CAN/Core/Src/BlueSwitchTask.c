/*
 * BlueSwitchTask.c
 *
 *  Created on: Nov. 23, 2022
 *      Author: marce
 */

#include "BlueSwitchTask.h"

static const uint32_t BLUE_LED_EXID = 0xAAAAAAA;

void blueSwitchTask(void const* arg)
{
    //One time osDelayUntil initialization
    uint32_t prevWakeTime = osKernelSysTick();

    uint8_t data[1] = {0x89};

    for (;;)
    {
    	osDelay(10);
    	if (HAL_GPIO_ReadPin(BLUE_SWITCH_GPIO_Port, BLUE_SWITCH_Pin)) {
    		data[0] = 0x89;
    	} else {
    		data[0] = 0x0;
    	}

    	if (osMutexWait(SPIMutexHandle, 0) == osOK) {
    		sendExtendedCANMessage(0, BLUE_LED_EXID, 1, data);
			osMutexRelease(SPIMutexHandle);
    	}
    }
}
