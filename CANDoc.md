# CAN Driver Comparison...

---

## sendCANMessage() & sendExtendedCANMessage

Radio Drivers:

    - specify channel to send message to
    - takes in field values for message, like channel or DLC

Helios Embedded Drivers:

    - finds available channel using `checkAvailableTXChannel()`
    - takes in complete message

## CANRxInterruptTask()

Pretty much the same, the one on Radio Drivers sends stuff over the radio after receiving CAN message.

## TODOs?

- Function that checks available channels and sends the relevant `sendCANMessage` function (Extended vs standard message)
    - Note: do you mean like find what kind of can message to send

- Make a `CanTxGatekeeperTask` to ensure only one task can access SPI-CAN peripheral at once, also test it by make a freertos task handle

- Create a queue if there are no available channels

- Test the stuff...

- Comment the fuck outta this, also a READMe to help understand 


## Other stuff?
On helios embedded (line 31), why is size of list 3 but only initialized to two values? also why 0x03?
    - Solved: Just cause, 0x03 is just to specify its a read instruction for the spi interface, only 3 bytes are needed, 1st byte is instruction byte (0x03), 2nd byte is the address of buffer, 3rd is a "dont care"

Controller SPI interface instructions like read and write seems fine

- Read instruction
- Write instruction
- Bitwise write instruction

---

## More TODOs...

- Tx task, make sure 1 transmit at a time
- use the struct...
- double check addresses, and logic stuff...

---

## How Transmit works...

TXBnCTRL (1 byte)
Message arbitration (5 byte)
Message data (8 bytes)
