#include "MotorTestTask.h"

void motorTestTask() {

	MotorInfo motorInfo;

	//todo: have an emergency thing that sets everything to 0
	// another button that is an emergency brake tht sets everything to 0

	//if button set, get the speed value
	if (HAL_GPIO_ReadPin(SPEED_TORQUE_GPIO_Port, SPEED_TORQUE_Pin)) {
		motorInfo.controlValue = getSpeed();
		motorInfo.controlMode = 2;
	} else {
		motorInfo.controlValue = getTorque();
		motorInfo.controlMode = 1;
	}

	sendMotorInfo(motorInfo);

}


void sendMotorInfo(MotorInfo* motorInfo) {

	uint8_t data[8];

	//break the value into 2 as byte 1 and 2 is for the speed/torque values
	data[0] = motorInfo->controlValue && 0xFF;
	data[1] = motorInfo->controlValue >> 8;

	// third byte:
	// bit 0, 1 for controlMode
	// bit 2 - 4 for motorMode
	// bit 5 for swEnable
	// bit 7 for debug mode

	uint8_t mode;
	mode = motorInfo->controlMode;
	mode |= motorInfo->motorMode << 2;
	mode |= motorInfo->swEnable << 5;
	mode |= motorInfo->debugMode << 7;

	data[2] = mode;

	CANMsg msg = {
		8,
		//if set, control motor 1, otherwise motor 2
		HAL_GPIO_ReadPin(MOTOR_CONTROL_GPIO_Port, MOTOR_CONTROL_Pin) ? MOTOR_0 : MOTOR_1,
		0,
		data
	};

	sendCANMessage(&msg);
}
