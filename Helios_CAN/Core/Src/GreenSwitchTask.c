/*
 * GreenSwitchTask.c
 *
 *  Created on: Nov. 23, 2022
 *      Author: marce
 */

#include "GreenSwitchTask.h"

static const uint32_t GREEN_LED_EXID = 0xBBBBBBB;

void greenSwitchTask(void const* arg)
{
    //One time osDelayUntil initialization
    uint32_t prevWakeTime = osKernelSysTick();

    uint8_t data[1] = {0x3};

    for (;;)
    {
    	osDelay(10);
    	if (HAL_GPIO_ReadPin(GREEN_SWITCH_GPIO_Port, GREEN_SWITCH_Pin)) {
    		data[0] = 0x3;
    	} else {
    		data[0] = 0x0;
    	}

    	if (osMutexWait(SPIMutexHandle, 0) == osOK) {
			sendExtendedCANMessage(1, GREEN_LED_EXID, 1, data);
		}

    	osMutexRelease(SPIMutexHandle);
    }
}
