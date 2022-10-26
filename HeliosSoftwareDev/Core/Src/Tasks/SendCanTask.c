#include "SendCanTask.h"

void sendCanTask(void* arg) {
    // SPI_HandleTypeDef spiModule;

    // uint8_t pData = 1000001;    //RTS to TXB0 buffer
    // uint16_t size = 

    // HAL_SPI_Transmit();

    // Write to transmit buffer 0
    // 1. Write to TXB0SIDH
    // 2. Write to TXB0SIDL
    // 3. Write to TXB0DLC  // should be 1
    // 4. Write to TXB0D0 // first byte of data register, send 0000 00001. "data[0] = 1"
    // 5. Set TXREQ to flag msg for transmission

    uint8_t data[8];

    for (;;) {



    }
}