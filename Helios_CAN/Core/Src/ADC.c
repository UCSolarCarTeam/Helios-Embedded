#include "ADC.h"

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

uint32_t getVoltage() {
    
    uint32_t voltage = 0;

    if (HAL_ADC_Start(&hadc) == HAL_OK) {
            if (HAL_ADC_PollForConversion(&hadc, ADC_TIMEOUT) == HAL_OK) {
                voltage = HAL_ADC_GetValue(&hadc);
            }       

        HAL_ADC_Stop(&hadc);
    }

    //todo: do something with the value to get the voltage
    return voltage;
}

void sendVoltage() {
    //todo: send the voltage value through can to the motors
    //todo: just send it as data to 0x012?

    uint32_t voltage = getVoltage();

    //todo actually properly do these stuff
    CANMsg msg = {
        8, 
        0x012,
        0,
        voltage;
    };

    sendCANMessage(&msg);

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



}