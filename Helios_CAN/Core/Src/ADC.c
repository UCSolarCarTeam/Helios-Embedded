#include "ADC.h"
#include <math.h>

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

    // 3.3 / 2^(adc bits) = voltageStep
    // voltage = binaryVoltage * voltageStep
    float voltage = binaryVoltage * 3.09f / powf(2.0, 12.0);	//3.09 because we get the most accuracy with it however it should be 3 or 3.30

    voltage = voltage / MAX_VOLTAGE; // scale voltage to a decimal

    return voltage;
}


int16_t getSpeed() {
    float volt = getVoltage();

    //Scaling factor between 0 and 1 to control the size of the range
    float rangeScalingFactor = 1;

    float maxSpeed = MAX_SPEED * rangeScalingFactor;
    float minSpeed = MIN_SPEED * rangeScalingFactor;

    return volt * (maxSpeed - minSpeed) + minSpeed;
}

int16_t getTorque() {
    float volt = getVoltage();

    //Scaling factor between 0 and 1 to control the size of the range
    float rangeScalingFactor = 1;

    float maxTorque = MAX_TORQUE * rangeScalingFactor;
    float minTorque = MIN_TORQUE * rangeScalingFactor;

    return volt * (maxTorque - minTorque) + minTorque;
}
