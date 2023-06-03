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
