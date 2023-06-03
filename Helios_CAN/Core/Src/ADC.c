#include "ADC.h"
#include <math.h>

// ADC conversion by polling:
// - Activate the ADC peripheral and start conversions using function HAL_ADC_Start()
// - Wait for ADC conversion completion using function HAL_ADC_PollForConversion() (or for injected group: HAL_ADCEx_InjectedPollForConversion() )
// - Retrieve conversion results using function HAL_ADC_GetValue() (or for injected group: HAL_ADCEx_InjectedGetValue() )
// - Stop conversion and disable the ADC peripheral using function HAL_ADC_Stop() 

//get info, decide min and max voltage and send it to the motors
//check the motor information for the cna message ids
// create buffer or somting

#define ADC_TIMEOUT 10      //in milliseconds
#define ADC_DELAY 10


//min voltage = 0
// max voltage = 3.09
float getVoltage() {
    
    uint32_t binaryVoltage = 0;

    if (HAL_ADC_Start(&hadc) == HAL_OK) {
            if (HAL_ADC_PollForConversion(&hadc, ADC_TIMEOUT) == HAL_OK) {
            	binaryVoltage = HAL_ADC_GetValue(&hadc);
            }       

        HAL_ADC_Stop(&hadc);
    }

    //todo: do something with the value to get the voltage

    float voltage = binaryVoltage * 3.09f / powf(2.0, 12.0);
    // 3.3 / 2^(adc bits) = voltagestep
    // voltage * voltageStep

    //todo: the adc manual, check the voltage reference.
    // check if there is an output pin that i can probe and see what the voltage reference is actually?

    //because this 3.09 should 3 or 3.3 because that is what the stm board takes in (marcelo's assumption)
    // 3.12

    voltage = voltage / MAX_VOLTAGE; // scale voltage to a decimal

    return voltage;
}


//todo: scale voltage down to a percentage!!

//ADC to speed, signed 16 bit -10000 to 10000 for Speed control 10 * RPM
// Map to min and max voltage
int16_t getSpeed() {
    float volt = getVoltage();

//    float slope = 1.0 * (MAX_SPEED - MIN_SPEED) / (MAX_VOLTAGE - MIN_VOLTAGE);
//
//    return MIN_SPEED + slope * (volt - MIN_VOTLAGE);

    //make a scaling factor so that we can just change the scaling factor
    float rangeScalingFactor = 1;

    float maxSpeed = MAX_SPEED * rangeScalingFactor;
    float minSpeed = MIN_SPEED * rangeScalingFactor;

    return volt * (maxSpeed - minSpeed) + minSpeed;

}

int16_t getTorque() {
    float volt = getVoltage();

//    float slope = 1.0 * (MAX_TORQUE - MIN_TORQUE) / (MAX_VOLTAGE - MIN_VOLTAGE);
//
//    return MIN_TORQUE + slope * (volt - MIN_VOTLAGE);

    float rangeScalingFactor = 1;

    float maxSpeed = MAX_TORQUE * rangeScalingFactor;
    float minSpeed = MIN_TORQUE * rangeScalingFactor;

    return volt * (maxSpeed - minSpeed) + minSpeed;

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
		0x012,		//todo: make a macro to differentiate between motor 1 and 2 address, values should come from electrical
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


void sendVoltage() {

    //todo: send the voltage value through can to the motors
    //todo: just send it as data to 0x012?

//    uint32_t voltage = getVoltage();
//
//    uint8_t data[8];
//
//    data[0] = voltage;
//
//    //todo actually properly do these stuff
//    CANMsg msg = {
//        8,
//        0x012,
//        0,
//        data
//    };
//
//    sendCANMessage(&msg);

    //can info: 
    // • protocol: CAN 2.0 A,
    // • typical baud rate: 125 kbps, (125, 250, 500)
    // • RX period: 10 ms,
    // • TX period: one message every RX period (cyclic),
    // • Endianess format: Intel
    // • RX and TX addresses can be set via user setting (GEM Flash tool), standard setting ID=0x012
    // (hex) and Baud rate=125000
    // • 0x100 (Hex) and 0x101 (hex) CAN addresses are reserved for motor bootloader
    // communication

	//control using speed mode, 0 min 3.09V max speed.

}
