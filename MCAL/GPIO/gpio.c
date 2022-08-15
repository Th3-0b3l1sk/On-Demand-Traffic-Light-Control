/* -------------------------------------
* Author: Mahmoud Abdullah
* EgFWD Professional Embedded Systems
* Github: https://github.com/Th3-0b3l1sk
* --------------------------------------
*/

#include "./gpio.h"
#include "../../Util/bits.h"

////////////////////////////////////////////////////////////////////
//	Global Static Objects
////////////////////////////////////////////////////////////////////

/*
* Instead of using a switch clause to each possible 
* port register, those arrays are used to directly 
* index the required register using the port number.
*
* MAINTAIN THE SAME ORDER OF THE ELEMENTS AS IN THE IO_Port ENUM
*/
static uint8_t* DDR[4]  = { DRA, DRB, DRC, DRD };
static uint8_t* PORT[4] = { PRA, PRB, PRC, PRD };
static uint8_t* PIN[4]  = { PNA, PNB, PNC, PND };

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
//	Function Definitions
////////////////////////////////////////////////////////////////////

GPIO_STATE    IO_PortInit  (const IO_Port _port, IO_Dir _dir)
{
	if(_port > 3 )
	return;
	
	switch(_dir){
		case DIR_OUT:
			*DDR[_port] = 0xFF;
			break;
		case DIR_IN:
			*DDR[_port] = 0x00;
			break;
		default:
			return GPIO_FALIURE;
	}
	
	return GPIO_SUCCESS;
}

GPIO_STATE    IO_PortWrite (const IO_Port _port, const uint8_t _value)
{
	if(_port > 3)
		return GPIO_FALIURE;
	
	*PORT[_port] = _value;
	return GPIO_SUCCESS;
}

GPIO_STATE	  IO_PortToggle(const IO_Port _port)
{
	if(_port > 3)
	return GPIO_FALIURE;
	
	*PORT[_port] ^= 0xFF;
	return GPIO_SUCCESS;
}

uint8_t		  IO_PortRead  (const IO_Port _port, uint8_t* _value)
{
	if(_port > 3 || _value == NULL)
		return GPIO_FALIURE;
	
	*_value = *PIN[_port];
	return GPIO_SUCCESS;
	
}

GPIO_STATE    IO_PinInit   (const IO_Port _port, const uint8_t _pin, IO_Dir _dir)
{
	if(_port > 3 || _pin > 7)
		return GPIO_FALIURE;
	
	switch(_dir){
		case DIR_IN:
			BIT_CLEAR(*DDR[_port], _pin);
			break;
		case DIR_OUT:
			BIT_WRITE(*DDR[_port], _pin);
			break;
		default:
			return GPIO_FALIURE;
	}
	
	return GPIO_SUCCESS;
}

GPIO_STATE    IO_PinWrite  (const IO_Port _port, const uint8_t _pin, const uint8_t _value)
{
	if(_port > 3 || _pin > 7)
		return GPIO_FALIURE;
	
	switch(_value){
		case LOW:
			BIT_CLEAR(*PORT[_port], _pin);
			break;
		case HIGH:
			BIT_WRITE(*PORT[_port], _pin);
			break;
		default:
			return GPIO_FALIURE;
	}
	
	return GPIO_SUCCESS;
}

GPIO_STATE	  IO_PinToggle (const IO_Port _port, const uint8_t _pin)
{
	if(_port > 3)
		return GPIO_FALIURE;
		
	BIT_TOGGLE(*PORT[_port], _pin);
	
	return GPIO_SUCCESS;
}

uint8_t		  IO_PinRead   (const IO_Port _port, const uint8_t _pin, uint8_t* _value)
{
	if(_port > 3 || _pin > 7 || _value == NULL)
	return GPIO_FALIURE;
	
	*_value =  BIT_READ(*PIN[_port], _pin);
	
	return GPIO_SUCCESS;
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////