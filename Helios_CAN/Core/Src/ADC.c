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
    return voltage;
}


//ADC to speed, signed 16 bit -10000 to 10000 for Speed control 10 * RPM
// Map to min and max voltage
int16_t getSpeed() {
    float volt = getVoltage();

    float slope = 1.0 * (MAX_SPEED - MIN_SPEED) / (MAX_VOLTAGE - MIN_VOLTAGE);

    return MIN_SPEED + slope * (volt - MIN_VOTLAGE);
}

int16_t getTorque() {
    float volt = getVoltage();

    float slope = 1.0 * (MAX_TORQUE - MIN_TORQUE) / (MAX_VOLTAGE - MIN_VOLTAGE);

    return MIN_TORQUE + slope * (volt - MIN_VOTLAGE);
}

//ADC to torque
//in percentages -100 to 100
//int16_t getTorque() {
//
//
//
//}

//float getCurrent() {
    // To avoid a software overcurrent, our motor config
    // is set to have 100 A max current, we scale it so we send 69 A
    // on full pedal press

    // if ((accelPercentage - NON_ZERO_THRESHOLD) > 0 )
    // {
    //     return (accelPercentage - NON_ZERO_THRESHOLD)
    //            / (MAX_PEDAL_THRESHOLD - NON_ZERO_THRESHOLD)
    //            * MOTOR_PERCENTAGE_REDUCER;
    // }
    // else
    // {
    //     return 0.0;
    // }
//}

void sendMotorInfo(MotorInfo* motorInfo) {
	int16_t speed = getSpeed();
	int16_t torque = getTorque();

	//need to check if button set, if button set do speed?
	if (HAL_GPIO_ReadPin(SPEED_TORQUE_GPIO_Port, SPEED_TORQUE_Pin)) {
		motorInfo->controlValue = speed;
		motorInfo->controlMode = 2;

	} else {
		motorInfo->controlValue = torque;
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
	mode |= motorInfo>debugMode << 7;

	data[2] = mode;

	CANMsg msg = {
		8,
		0x012,
		0,
		data
	};

	sendCANMessage(&msg);
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
