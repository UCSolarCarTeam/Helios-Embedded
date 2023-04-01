#pragma once

#include "main.h"
#include <stdint.h>
#include "cmsis_os.h"
#include "CANRegisters.h"

typedef struct {
    uint8_t DLC;
    uint16_t ID;
    uint64_t extendedID;
    uint8_t data[8];
} CANMsg;

// https://www.codesdope.com/blog/article/making-a-queue-using-linked-list-in-c/

#define TX_CHANNEL_CHECK_DELAY 10

void CAN_IC_READ_REGISTER(uint8_t address, uint8_t* buffer);
void CAN_IC_WRITE_REGISTER_BITWISE(uint8_t address, uint8_t mask, uint8_t value);
void CAN_IC_WRITE_REGISTER(uint8_t address, uint8_t value);
void ConfigureCANSPI(void);
void sendCANMessage(CANMsg *msg);
void sendExtendedCANMessage(uint8_t channel, uint64_t ID, uint8_t DLC, uint8_t* data);
void receiveCANMessage(uint8_t channel, uint32_t* ID, uint8_t* DLC, uint8_t* data);

extern SPI_HandleTypeDef hspi1;
extern osMutexId_t SPIMutexHandle;
extern osMessageQueueId_t CANInterruptQueue;
extern osMessageQueueId_t CANTxMessageQueue;

extern uint8_t blueStatus;
extern uint8_t greenStatus;