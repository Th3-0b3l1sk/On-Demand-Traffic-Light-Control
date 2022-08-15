/* -------------------------------------
* Author: Mahmoud Abdullah
* EgFWD Professional Embedded Systems
* Github: https://github.com/Th3-0b3l1sk
* --------------------------------------
*/


#ifndef LED_H_
#define LED_H_

#include "../../MCAL/GPIO/gpio.h"
#include "../../Util/defs.h"

typedef enum LED_STATE
{
	LED_SUCCESS = 1,
	LED_FAILURE = 0
	
} LED_STATE;


typedef struct st_Led
{
	uint8_t port;
	uint8_t pin;
	
} st_Led;		

/*
* Initializes the led on the selected port and pin.
*
* @param[in]	_led	a pointer to an st_Led structure.
* @return		LED_SUCCESS on success, LED_FAILURE otherwise
*/
LED_STATE LED_Init(st_Led* _led);

/*
* Switches the led on(writes logic high).
*
* @param[in]	_led	a pointer to an st_Led structure.
* @return		LED_SUCCESS on success, LED_FAILURE otherwise
*/
LED_STATE LED_On(st_Led* _led);

/*
* Switches the led off(writes logic low).
*
* @param[in]	_led	a pointer to an st_Led structure.
* @return		LED_SUCCESS on success, LED_FAILURE otherwise
*/
LED_STATE LED_Off(st_Led* _led);

/*
* Toggles the led on/off.
*
* @param[in]	_led	a pointer to an st_Led structure.
* @return		LED_SUCCESS on success, LED_FAILURE otherwise
*/
LED_STATE LED_Toggle(st_Led* _led);


#endif /* LED_H_ */