/*
 * ReadWriteEEPROMTask.c
*/

#include "ReadWriteEEPROMTask.h"
#define LED_RED_GPIO_Port GPIOC

void readWriteEEPROMTask(void const* arg)
{
  uint32_t prevWakeTime = osKernelSysTick();
  
  for (;;)
  {
    char some_array[7] = "Testing";
    uint8_t *data = (uint8_t*)some_array;
    uint16_t len = 7;

    /*
     * write
    */
    uint16_t i;
    HAL_StatusTypeDef status = HAL_OK;

    HAL_FLASHEx_DATAEEPROM_Unlock();
    for(i=0;i<len;i++)
    {
        status += HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, EEPROM_BASE_ADDR+i, *data);
        data++;
    }
    HAL_FLASHEx_DATAEEPROM_Lock();

    HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);

    /*
     * read
    */
    uint8_t *buffer;
    uint8_t *wAddr;
    wAddr=(uint8_t *)(EEPROM_BASE_ADDR);
    while(len--)
    {
        *buffer++=*wAddr++;
    }
  }
}

