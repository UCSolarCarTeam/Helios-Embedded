#pragma once

#define EEPROM_BASE_ADDR 0x08080100
#define EEPROM_BYTE_SIZE 0x03FF

#include "stm32l1xx.h"
#include "stm32l1xx_hal_gpio.h"
#include "cmsis_os.h"
#include "main.h"

void readWriteEEPROMTask(void const* arg);