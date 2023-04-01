#pragma once
#include <stdint.h>
#include "main.h"
#include "stm32l1xx_hal_adc.h"
#include "CAN.h"

extern ADC_HandleTypeDef hadc;

float getVoltage();
