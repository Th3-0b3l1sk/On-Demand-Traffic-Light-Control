/* -------------------------------------
* Author: Mahmoud Abdullah
* EgFWD Professional Embedded Systems
* Github: https://github.com/Th3-0b3l1sk
* --------------------------------------
*/

#ifndef TEST_BUTTON_H_
#define TEST_BUTTON_H_

#include <stdio.h>
#include "./button.h"

////////////////////////////////////////////////////////////////////
//	Test Functions
////////////////////////////////////////////////////////////////////

BUTTON_STATE test_button_init()
{
	return BUTTON_Init(PORT_B, PIN_0);	
}

BUTTON_STATE test_button_state(en_BUTTON_SETUP _setup)
{
	
	st_Button button = {.port = PORT_B, .pin = PIN_0, .setup = _setup };
	BUTTON_STATE state = BUTTON_SUCCESS;
	
	state = BUTTON_Init_p(&button);
	if(state == BUTTON_FAILURE)
		return BUTTON_FAILURE;
		
	return BUTTON_State(&button);

	
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

#endif /* TEST_BUTTON_H_ */