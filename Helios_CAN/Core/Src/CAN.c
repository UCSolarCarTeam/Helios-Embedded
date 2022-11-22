#include "CAN.h"

/**
 * @brief write to registry in CAN IC
 * @param address: hex address of the register
 * 		  value: value to be written to the register
 * @retval None
 */
void CAN_IC_READ_REGISTER(uint8_t address, uint8_t* buffer)
{
	uint8_t packet[3] = {0x03, address};

	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, packet, 2, 100U);
	HAL_SPI_Receive(&hspi1, buffer, 1, 100U);
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
}

/**
 * @brief write to a spicific series of bits in a register in CAN IC
 * @param address: hex address of the register
 * 		  mask: bit mask
 * 		  value: value to be written to the register
 * @retval None
 */
void CAN_IC_WRITE_REGISTER_BITWISE(uint8_t address, uint8_t mask, uint8_t value)
{
	uint8_t packet[4] = {0x05, address, mask, value};

	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, packet, 4, 100U);
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
}

/**
 * @brief write to registry in CAN IC
 * @param address: hex address of the register
 * 		  value: value to be written to the register
 * @retval None
 */
void CAN_IC_WRITE_REGISTER(uint8_t address, uint8_t value)
{
	uint8_t packet[3] = {0x02, address, value};

	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, packet, 3, 100U);
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
}

/**
  * @brief configure CAN IC through SPI
  * @param None
  * @retval None
  * Configuration is as close to Elysia's CAN configuration whenever possible
  * TODO: add configuration verification and return value accordingly
  */
void ConfigureCANSPI(void)
{
	uint8_t resetCommand = 0xa0; //instruction to reset IC to default
	uint8_t CNF1 = 0x00; //BRP = 0 to make tq = 250ns and a SJW of 1Tq
	uint8_t CNF2 = 0x98; //PRSEG = 0, PHSEG1 = 3, SAM = 0, BTLMODE = 1
	uint8_t CNF3 = 0x01; //WAFKIL disabled, PHSEG2 = 2 (BTL enabled) but PHSEG = 1 makes it backwards compatible???? wat

	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, &resetCommand, 1, 100U);  //reset IC to default
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);

	CAN_IC_WRITE_REGISTER(0x0f, 0x80); //Ensure IC is in configuration mode

	CAN_IC_WRITE_REGISTER(0x2a, CNF1); //configure CNF1
	CAN_IC_WRITE_REGISTER(0x29, CNF2); //configure CNF2
	CAN_IC_WRITE_REGISTER(0x28, CNF3); //configure CNF3

	CAN_IC_WRITE_REGISTER(0x2b, 0xff); //configure interrupts, currently enable them all
	CAN_IC_WRITE_REGISTER(0x2C, 0x00); //clear INTE flags
									   //this should be a bit-wise clear in any other case to avoid unintentionally clearing flags

	CAN_IC_WRITE_REGISTER(0x0f, 0x04); //Put IC in normal operation mode with CLKOUT pin enable and 1:1 prescaler
}

/**
  * @brief send CAN message
  * @param None
  * @retval None
  */
void sendCANMessage(uint8_t channel, uint16_t ID, uint8_t DLC, uint8_t* data)
{
	uint8_t initialBufferAddress = 0x20 + 16*(channel); //0x30 for channel 1, 0x40 for channel 2, 0x50 for channel 3

	uint8_t sendCommand = 0x81; //instruction to send CAN message on buffer 1

	uint8_t TXBNSIDH = (ID & 0b11111111000) >> 3;
	uint8_t TXBNSIDL = ((ID & 0b111) << 5);
	uint8_t TXBNDLC = DLC & 0x0f;

	CAN_IC_WRITE_REGISTER(initialBufferAddress + 1, TXBNSIDH); // SD 10-3
	CAN_IC_WRITE_REGISTER(initialBufferAddress + 2, TXBNSIDL); //SD 2-0
	CAN_IC_WRITE_REGISTER(initialBufferAddress + 5, TXBNDLC);  //DLC

	uint8_t initialDataBufferAddress = initialBufferAddress + 6;
	for(int i = 0; i < DLC; i++)
	{
		CAN_IC_WRITE_REGISTER(initialDataBufferAddress + i, data[i]); //write to relevant data registers
	}

	CAN_IC_WRITE_REGISTER_BITWISE(initialBufferAddress, 0x02, 0x02); //set transmit buffer priority to 4 (max)

	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, &sendCommand, 1, 100U);  //Send command to transmit
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
}

/**
  * @brief send CAN message
  * @param None
  * @retval None
  */
void sendExtendedCANMessage(uint8_t channel, uint64_t ID, uint8_t DLC, uint8_t* data)
{
	uint8_t initialBufferAddress = 0x20 + 16*(channel); //0x30 for channel 1, 0x40 for channel 2, 0x50 for channel 3

	uint8_t sendCommand = 0x81; //instruction to send CAN message on buffer 1

	uint8_t TXBNEID0 = ID & 0xff;
	uint8_t TXBNEID8 = (ID >> 8) & 0xff;
	uint8_t TXBNSIDH = (ID >> 21) & 0xff;
	uint8_t TXBNSIDL = ((ID >> 18) & 0x07) | 0b00001000 | ((ID >> 16) & 0x03);
	uint8_t TXBNDLC = DLC & 0x0f;

	CAN_IC_WRITE_REGISTER(initialBufferAddress + 1, TXBNSIDH); // SD 10-3
	CAN_IC_WRITE_REGISTER(initialBufferAddress + 2, TXBNSIDL); //SD 2-0, ED 17-16
	CAN_IC_WRITE_REGISTER(initialBufferAddress + 3, TXBNEID8); //ED 15-8
	CAN_IC_WRITE_REGISTER(initialBufferAddress + 4, TXBNEID0); //ED 7-0
	CAN_IC_WRITE_REGISTER(initialBufferAddress + 5, TXBNDLC);  //DLC

	uint8_t initialDataBufferAddress = initialBufferAddress + 6;
	for(int i = 0; i < DLC; i++)
	{
		CAN_IC_WRITE_REGISTER(initialDataBufferAddress + i, data[i]); //write to relevant data registers
	}

	CAN_IC_WRITE_REGISTER_BITWISE(initialBufferAddress, 0x02, 0x02); //set transmit buffer priority to 4 (max)

	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, &sendCommand, 1, 100U);  //Send command to transmit
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
}
