/* -------------------------------------
* Author: Mahmoud Abdullah
* EgFWD Professional Embedded Systems
* Github: https://github.com/Th3-0b3l1sk
* --------------------------------------
*/

#ifndef TEST_LED_H_
#define TEST_LED_H_

#include "led.h"

////////////////////////////////////////////////////////////////////
//	Test Functions
////////////////////////////////////////////////////////////////////

LED_STATE test_led_init()
{
	st_Led _led = { .port = PORT_B, .pin = PIN_0 };
	return LED_Init(&_led);
}

LED_STATE test_led_on()
{
	st_Led _led = { .port = PORT_B, .pin = PIN_0 };
	LED_STATE status = LED_SUCCESS;
	
	status = LED_Init(&_led);
	if(status == LED_FAILURE)
		return LED_FAILURE;
		
	return LED_On(&_led);
	
}

LED_STATE test_led_off()
{
	st_Led _led = { .port = PORT_B, .pin = PIN_0 };
	LED_STATE status = LED_SUCCESS;
	
	status = LED_Init(&_led);
	if(status == LED_FAILURE)
		return LED_FAILURE;
	
	return LED_Off(&_led);	
}

LED_STATE test_led_toggle()
{
	st_Led _led = { .port = PORT_B, .pin = PIN_0 };
	LED_STATE status = LED_SUCCESS;
	
	status = LED_Init(&_led);
	if(status == LED_FAILURE)
		return LED_FAILURE;
	
	return LED_Toggle(&_led);
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

#endif /* TEST_LED_H_ */