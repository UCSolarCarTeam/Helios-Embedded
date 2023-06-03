#include "MotorTestTask.h"

//where we actually do the setting and stuff
//need to receive from the CAN message as well

//Set torque for one motor and the other speed?
void motorTestTask() {

	MotorInfo motorInfo;

	sendMotorInfo(motorInfo);

}


void sendMotorInfo(MotorInfo* motorInfo) {

	//need to check if button set, if button set do speed?
	if (HAL_GPIO_ReadPin(SPEED_TORQUE_GPIO_Port, SPEED_TORQUE_Pin)) {
		motorInfo->controlValue = getSpeed();
		motorInfo->controlMode = 2;
	} else {
		motorInfo->controlValue = getTorque();
		motorInfo->controlMode = 1;
	}

	uint8_t data[8];

	//break the value into 2 as byte 1 and 2 is for the speed

	data[0] = motorInfo->controlValue && 0xFF;
	data[1] = motorInfo->controlValue >> 8;

	// third one
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
		//if set, it is motor 1, otherwise motor 2
		HAL_GPIO_ReadPin(MOTOR_CONTROL_GPIO_Port, MOTOR_CONTROL_Pin) ? MOTOR_0 : MOTOR_1,
					//todo: make a macro to differentiate between motor 1 and 2 address, values should come from electrical
					//todo: button motor one or motor 2 to be controlled
					// todo: make the task to do the sloshing
					// the one being controlled gets 0 torque, the other one gets whatever (the actual pretention meter thing)

					//task starting
					// input torque value for one motor
					// we decide speed value and regen for the other
					//torque in mechanical is like current is electrical
					// check that graph to see if we can use the graph to decide what speed values it should have based on the torque and te current
					// slowly decrease
		0,
		data
	};

	sendCANMessage(&msg);

	//todo: have an emergency thing that sets everything to 0
	// another button that is an emergency brake tht sets everything to 0
}
