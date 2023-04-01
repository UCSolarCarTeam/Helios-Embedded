#pragma once
#include <stdint.h>
#include "main.h"
#include "stm32l1xx_hal_adc.h"

extern ADC_HandleTypeDef hadc;

uint32_t getVoltage();  