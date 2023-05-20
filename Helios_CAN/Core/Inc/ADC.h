#pragma once
#include <stdint.h>
#include "main.h"
#include "stm32l1xx_hal_adc.h"
#include "CAN.h"

#define MAX_ANALOG 4096 // 2^12
#define MAX_VOLTAGE 3.09f
#define MIN_VOLTAGE 0
#define MAX_SPEED 10000
#define MIN_SPEED -10000

extern ADC_HandleTypeDef hadc;

float getVoltage();
