/* -------------------------------------
* Author: Mahmoud Abdullah
* EgFWD Professional Embedded Systems
* Github: https://github.com/Th3-0b3l1sk
* --------------------------------------
*/

#include "led.h"
#include "../../MCAL/GPIO/gpio.h"

LED_STATE LED_Init(st_Led* _led)
{
	if(_led == NULL) return LED_FAILURE;
	return IO_PinInit(_led->port, _led->pin, DIR_OUT);
}

LED_STATE LED_On(st_Led* _led)
{
	if(_led == NULL) return LED_FAILURE;
	return IO_PinWrite(_led->port, _led->pin, HIGH);
}

LED_STATE LED_Off(st_Led* _led)
{
	if(_led == NULL) return LED_FAILURE;
	return IO_PinWrite(_led->port, _led->pin, LOW);
}

LED_STATE LED_Toggle(st_Led* _led)
{
	if(_led == NULL) return LED_FAILURE;
	return IO_PinToggle(_led->port, _led->pin);
}

