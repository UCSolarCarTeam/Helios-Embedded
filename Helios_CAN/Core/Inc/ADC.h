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
#define MAX_TORQUE 100
#define MIN_TORQUE -100


extern ADC_HandleTypeDef hadc;

float getVoltage();


struct MotorInfo {
	//input to motors
	int16_t controlValue;
	uint8_t controlMode;
	uint8_t motorMode;
	uint8_t swEnable;
	uint8_t debugMode;

	//receive from motors
	uint8_t motorState;
	int16_t motorTorque;
	int16_t motorRpm;
	int16_t motorTemp;
	int16_t invPeakCurr;
	int16_t motorPower;
	uint16_t absPosition;
	uint64_t warningCode;
	uint64_t errorCode;
};
