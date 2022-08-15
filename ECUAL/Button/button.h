/* -------------------------------------
* Author: Mahmoud Abdullah
* EgFWD Professional Embedded Systems
* Github: https://github.com/Th3-0b3l1sk
* --------------------------------------
*/


#ifndef BUTTON_H_
#define BUTTON_H_

#include "../../Util/defs.h"

////////////////////////////////////////////////////////////////////
//	Type Definitions
////////////////////////////////////////////////////////////////////

typedef enum BUTTON_STATE
{
	BUTTON_SUCCESS	 = 1,
	BUTTON_FAILURE	 = 0,
					 
	BUTTON_PRESSED	 = 3,
	BUTTON_UNPRESSED = 4
	
} BUTTON_STATE;

// whether the button is pulled high or low
typedef enum en_BUTTON_SETUP
{
	BUTTON_SETUP_HIGH = 0,
	BUTTON_SETUP_LOW  = 1
} en_BUTTON_SETUP;

typedef struct st_Button
{
	uint8_t port;
	uint8_t pin;
	en_BUTTON_SETUP setup;
} st_Button;

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
//	Function Declarations
////////////////////////////////////////////////////////////////////

/*
* Initialize the button on the selected port and pin.
*
* @param[in]	_port	the port number.
* @param[in]	_pin	the pin number.
* @return		BUTTON_SUCCESS on success, LED_FAILURE otherwise
*/
BUTTON_STATE BUTTON_Init(uint8_t _port, uint8_t _pin);


/*
* Initialize the button on the selected port and pin.
*
* @param[in]	_button		a pointer to a st_Button struct.
* @return		BUTTON_SUCCESS on success, LED_FAILURE otherwise
*/
BUTTON_STATE BUTTON_Init_p(st_Button* _button);


/*
* Retrieves the button state whether pressed or unpressed.
*
* @param[in]	_button		a pointer to a st_Button struct.
* @return		BUTTON_SUCCESS BUTTON_PRESSED, BUTTON_UNPRESSED on success or BUTTON_FAILURE on failure.
*/
BUTTON_STATE BUTTON_State(st_Button* _button);

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////



#endif /* BUTTON_H_ */