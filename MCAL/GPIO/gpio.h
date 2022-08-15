/* -------------------------------------
* Author: Mahmoud Abdullah
* EgFWD Professional Embedded Systems
* Github: https://github.com/Th3-0b3l1sk
* --------------------------------------
*/

#ifndef GPIO_H_
#define GPIO_H_

#include "../../Util/defs.h"

////////////////////////////////////////////////////////////////////
//	Macros
////////////////////////////////////////////////////////////////////

/*
* Those definitions are made to get pointers to the port registers
* without dereferencing them, to facilitate the global arrays lookup
* in gpio.c
*/
#define DRA (uint8_t*) 0x3A
#define DRB (uint8_t*) 0x37
#define DRC (uint8_t*) 0x34
#define DRD (uint8_t*) 0x31
#define PRA (uint8_t*) 0x3B
#define PRB (uint8_t*) 0x38
#define PRC (uint8_t*) 0x35
#define PRD (uint8_t*) 0x32
#define PNA (uint8_t*) 0x39
#define PNB (uint8_t*) 0x36
#define PNC (uint8_t*) 0x33
#define PND (uint8_t*) 0x30

#define PIN_0 0		// uC Pin 0
#define PIN_1 1		// uC Pin 1
#define PIN_2 2		// uC Pin 2
#define PIN_3 3		// uC Pin 3
#define PIN_4 4		// uC Pin 4
#define PIN_5 5		// uC Pin 5
#define PIN_6 6		// uC Pin 6
#define PIN_7 7		// uC Pin 7

#define LOW  0		// Logic low
#define HIGH 1		// Logic high

#define DDRA   *( (volatile uint8_t*) DRA )		// Port-A Data Direction Register
#define PORTA  *( (volatile uint8_t*) PRA )		// Port-A Data Register
#define PINA   *( (volatile uint8_t*) PNA )		// Port-A Input Pins Register
#define DDRB   *( (volatile uint8_t*) DRB )		// Port-B Data Direction Register
#define PORTB  *( (volatile uint8_t*) PRB )		// Port-B Data Register
#define PINB   *( (volatile uint8_t*) PNB )		// Port-B Input Pins Register
#define DDRC   *( (volatile uint8_t*) DRC )		// Port-C Data Direction Register
#define PORTC  *( (volatile uint8_t*) PRC )		// Port-C Data Register
#define PINC   *( (volatile uint8_t*) PNC )		// Port-C Input Pins Register
#define DDRD   *( (volatile uint8_t*) DRD )		// Port-D Data Direction Register
#define PORTD  *( (volatile uint8_t*) PRD )		// Port-D Data Register
#define PIND   *( (volatile uint8_t*) PND )		// Port-D Input Pins Register

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
//	Type Definitions
////////////////////////////////////////////////////////////////////

typedef enum GPIO_STATE
{
	GPIO_SUCCESS = 1,
	GPIO_FALIURE = 0
	
} GPIO_STATE;

typedef enum IO_Dir		// IO Directions
{
	DIR_IN  = 0,
	DIR_OUT = 1
} IO_Dir;

typedef enum IO_Port	// 4 Possible ports of the atmega32 uC
{
	PORT_A = 0,
	PORT_B = 1,
	PORT_C = 2,
	PORT_D = 3
} IO_Port;


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
//	Function Declarations
////////////////////////////////////////////////////////////////////

/*
* Initializes the port and sets its direction.
*
* @param[in]	_port	The port.
* @param[in]	_dir	The direction.
* @return		GPIO_SUCCESS on success, GPIO_FAILURE otherwise
*/
GPIO_STATE	IO_PortInit	 (const IO_Port _port, IO_Dir _dir);

/*
* Writes to the port.
*
* @param[in]	_port	The port.
* @param[in]	_value	The value to write.
* @return		GPIO_SUCCESS on success, GPIO_FAILURE otherwise
*/
GPIO_STATE	IO_PortWrite (const IO_Port _port, const uint8_t _value);

/*
* Reads from the port.
*
* @param[in]	_port	The port.
* @param[out]	_value	A pointer to a uint8_t to write the value.
* @return		GPIO_SUCCESS on success, GPIO_FAILURE otherwise
*/
uint8_t IO_PortRead	 (const IO_Port _port, uint8_t* _value);

/*
* Toggles the port.
*
* @param[in]	_port	The port.
* @return		GPIO_SUCCESS on success, GPIO_FAILURE otherwise
*/
GPIO_STATE	IO_PortToggle(const IO_Port _port);

/*
* Initializes the pin and sets its direction.
*
* @param[in]	_port	The port.
* @param[in]	_pin	The pin.
* @param[in]	_dir	The direction.
* @return		GPIO_SUCCESS on success, GPIO_FAILURE otherwise
*/
GPIO_STATE	IO_PinInit	 (const IO_Port _port, const uint8_t _pin, IO_Dir _dir);

/*
* Writes to the pin.
*
* @param[in]	_port	The port.
* @param[in]	_pin	The pin.
* @param[in]	_value	The value to write.
* @return		GPIO_SUCCESS on success, GPIO_FAILURE otherwise
*/
GPIO_STATE	IO_PinWrite	 (const IO_Port _port, const uint8_t _pin, const uint8_t _value);	

/*
* Reads from the pin.
*
* @param[in]	_port	The port.
* @param[in]	_pin	The pin.
* @param[out]	_value	A pointer to a uint8_t to write the value.
* @return		GPIO_SUCCESS on success, GPIO_FAILURE otherwise
*/
uint8_t IO_PinRead	 (const IO_Port _port, const uint8_t _pin, uint8_t* _value);

/*
* Toggles the pin.
*
* @param[in]	_port	The port.
* @param[in]	_pin	The pin.
* @return		GPIO_SUCCESS on success, GPIO_FAILURE otherwise
*/
GPIO_STATE	IO_PinToggle (const IO_Port _port, const uint8_t _pin);

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

#endif /* GPIO_H_ */