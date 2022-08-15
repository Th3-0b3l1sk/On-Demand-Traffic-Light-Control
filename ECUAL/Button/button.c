/* -------------------------------------
* Author: Mahmoud Abdullah
* EgFWD Professional Embedded Systems
* Github: https://github.com/Th3-0b3l1sk
* --------------------------------------
*/

#include "button.h"
#include "../../MCAL/GPIO/gpio.h"


////////////////////////////////////////////////////////////////////
//	Function Declarations
////////////////////////////////////////////////////////////////////

BUTTON_STATE BUTTON_Init(uint8_t _port, uint8_t _pin)
{
	return IO_PinInit(_port, _pin, DIR_IN);
}

BUTTON_STATE BUTTON_Init_p(st_Button* _button)
{
	if(_button == NULL)
		return BUTTON_FAILURE;
		
	return IO_PinInit(_button->port, _button->pin, DIR_IN);
}

BUTTON_STATE BUTTON_State(st_Button* _button)
{
	/*
	* Depending on the button setup, a pressed button 
	* can be logic high or logic low
	**/
	
	if(_button == NULL)
		return BUTTON_FAILURE;
		
	// read the button pin
	uint8_t pinValue = 0x00;
	IO_PinRead(_button->port, _button->pin, &pinValue);
	
	// if the button is pulled low, then a push should pull it high and vice versa
	if(		(pinValue == HIGH) && (_button->setup == BUTTON_SETUP_LOW) 
		||	(pinValue == LOW)  && (_button->setup == BUTTON_SETUP_HIGH))
		return BUTTON_PRESSED;
	else 
		return BUTTON_UNPRESSED;	// any other combination is a released button
	
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////