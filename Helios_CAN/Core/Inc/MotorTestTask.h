#pragma once
#include "ADC.h"
#include "CAN.h"


#define MOTOR_0 0xDEAD
#define MOTOR_1	0xDEAD


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


void sendMotorInfo(MotorInfo* motorInfo);

void motorTestTask();
