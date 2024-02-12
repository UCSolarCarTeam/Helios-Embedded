/*
 * CANTransmitTask.c
 *
 *  Created on: Feb 10, 2024
 *      Author: MacKante
 */

#include "CANTxGatekeeperTask.h"
#include "CAN.h"

void CanTxGatekeeperTask(void* arg) {

    CANMsg newMsg;

    for (;;) {
        CanTxGatekeeper(&newMsg);
    }
}

void CanTxGatekeeper(CANMsg *msg) {
	// Acquire message to send from queue
	osMessageQueueGet(CANTxMessageQueue, msg, NULL, osWaitForever);

	// Wait for mutex
	if ( osMutexWait(SPIMutexHandle, 0) == osOK ) 
	{	
		// check if CAN message is standard/extended
		// if extendedID == 0, then message is standard
		if (msg->extendedID == 0)
		{
			sendCANMessage(msg);
		}
		else
		{
			sendExtendedCANMessage(msg);
		}
		osMutexRelease(SPIMutexHandle);
	}

}