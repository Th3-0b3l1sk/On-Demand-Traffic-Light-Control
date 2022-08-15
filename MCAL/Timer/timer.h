/* -------------------------------------
* Author: Mahmoud Abdullah
* EgFWD Professional Embedded Systems
* Github: https://github.com/Th3-0b3l1sk
* --------------------------------------
*/

#ifndef TIMER_H_
#define TIMER_H_

#include "../GPIO/gpio.h"
#include "../../Util/bits.h"
#include "../../Util/defs.h"

////////////////////////////////////////////////////////////////////
//	Macros
////////////////////////////////////////////////////////////////////

// general flags
#define TIMSK	*((volatile uint8_t*) 0x59)	// Timer/Counter Interrupt Mask Register
#define TIFR	*((volatile uint8_t*) 0x58)	// Timer/Counter Interrupt Flag Register

#define TOV0	0	// Timer Overflow (0)
#define OCF0	1	// Output Compare Match (0)
#define TOV1	2	// Timer Overflow (1)
#define OCF1B	3	// Output Compare Match (1B)
#define OCF1A	4	// Output Compare Match (1A)
#define ICF1	5	// Input Capture (1)
#define TOV2	6	// Timer Overflow (2)
#define OCF2	7	// Output Compare Match (2)

#define CS00	0	// Clock Select (00)
#define CS01	1	// Clock Select (01)
#define CS02	2	// Clock Select (02)
#define WGM01	3	// Wave Generation Mode (01)
#define COM00	4	// Compare Output Mode (00)
#define COM01	5	// Compare Output Mode (01)
#define WGM00	6	// Wave Generation Mode (00)

#define CS20	0	// Clock Select (20)
#define CS21	1	// Clock Select (21)
#define CS22	2	// Clock Select (22)
#define WGM21	3	// Wave Generation Mode (21)
#define COM20	4	// Compare Output Mode (20)
#define COM21	5	// Compare Output Mode (21)
#define WGM20	6	// Wave Generation Mode (20)

#define WGM10	0	// Wave Generation Mode (10)
#define WGM11	1	// Wave Generation Mode (11)
#define FOC1B	2	// Force Output Compare for Compare Unit B
#define FOC1A	3	// Force Output Compare for Compare Unit A
#define COM1B0	4	// Compare Output Mode for Compare Unit B (10)
#define COM1B1	5	// Compare Output Mode for Compare Unit B (11)
#define COM1A0	6	// Compare Output Mode for Compare Unit A (10)
#define COM1A1	7	// Compare Output Mode for Compare Unit A (11)

#define CS10	0	// Clock Select (10)
#define CS11	1	// Clock Select (11)
#define CS12	2	// Clock Select (12)
#define WGM12	3	// Wave Generation Mode (12)
#define WGM13	4	// Wave Generation Mode (13)
#define ICES1	6	// Input Capture Edge Select
#define ICNC1	7	// Input Capture Noise Canceler

#define TOIE0	0	// Timer/Counter0 Overflow Interrupt Enable
#define	OCIE0	1	// Timer/Counter0 Output Compare Match Interrupt Enable
#define TOIE1	2	// Timer/Counter1 Overflow Interrupt Enable
#define	OCIE1B	3	// Timer/Counter1 Output Compare B Match Interrupt Enable
#define OCIE1A	4	// Timer/Counter1 Output Compare A Match Interrupt Enable
#define	TICIE1	5	// Timer/Counter1 Input Capture Interrupt Enable
#define TOIE2	6	// Timer/Counter2 Overflow Interrupt Enable
#define OCIE2	7	// Timer/Counter2 Output Compare Match Interrupt Enable

// timer1 (16bit) - 2 independent output compare units
#define TCCR1A	*((volatile uint8_t*) 0x4F)	// Timer/Counter1 – Control Register A
#define TCCR1B	*((volatile uint8_t*) 0x4E)	// Timer/Counter1 – Control Register B
#define TCNT1H	*((volatile uint8_t*) 0x4D)	// Timer/Counter1 – Counter Register High Byte
#define TCNT1L	*((volatile uint8_t*) 0x4C)	// Timer/Counter1 – Counter Register Low Byte
#define OCR1AH	*((volatile uint8_t*) 0x4B)	// Timer/Counter1 – Output Compare Register A High Byte
#define OCR1AL	*((volatile uint8_t*) 0x4A)	// Timer/Counter1 – Output Compare Register A Low Byte
#define OCR1A	*((volatile uint16_t*) 0x4A)// Timer/Counter1 – Output Compare Register A Low Byte
#define OCR1BH	*((volatile uint8_t*) 0x49)	// Timer/Counter1 – Output Compare Register B High Byte
#define OCR1BL	*((volatile uint8_t*) 0x48)	// Timer/Counter1 – Output Compare Register B Low Byte
#define ICR1H	*((volatile uint8_t*) 0x47)	// Timer/Counter1 – Input Capture Register High Byte
#define ICR1L	*((volatile uint8_t*) 0x46)	// Timer/Counter1 – Input Capture Register Low Byte
#define ICR1A	*((volatile uint16_t*) 0x46)// Timer/Counter1 – Input Capture Register Low Byte

// timer0 (8bit)
#define TCCR0	*((volatile uint8_t*) 0x53)	// Timer/Counter0 - Control Register
#define TCNT0	*((volatile uint8_t*) 0x52)	// Timer/Counter0 - Counter Register
#define OCR0	*((volatile uint8_t*) 0x5C)	// Output Compare Register (0)

// timer2 (8bit)
#define TCCR2	*((volatile uint8_t*) 0x45)	// Timer/Counter2 - Control Register
#define TCNT2	*((volatile uint8_t*) 0x44)	// Timer/Counter2 - Counter Register
#define OCR2	*((volatile uint8_t*) 0x43)	// Output Compare Register (2)

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
//	Type Definitions
////////////////////////////////////////////////////////////////////

typedef enum TIM_STATE
{
	TIM_SUCCESS = 1,
	TIM_FAILURE = 0	
	
} TIM_STATE;

/* Timer/Counter 0 */
typedef enum en_WGM0	// Timer/Counter0 Wave Generation Flags
{
	WGM0_NORMAL = 0,
	WGM0_CTC,
	WGM0_FAST_PWM,
	WGM0_PHASE_CORRECT_PWM,
	
} en_WGM0;

typedef enum en_COM0	// Timer/Counter0 Compare Output Mode Flags
{
	COM0_DISCONNECTED = 0,
	
	COM0_NON_PWM_TOGGLE,
	COM0_NON_PWM_CLEAR,
	COM0_NON_PWM_SET,
	
	COM0_FAST_PWM_TOGGLE,
	COM0_FAST_PWM_NV,		// NON-INVERTING
	COM0_FAST_PWM_IV,		// INVERTING
	
	COM0_PHASE_CORRECT_TOGGLE,
	COM0_PHASE_CORRECT_NV,
	COM0_PHASE_CORRECT_IV
	
} en_COM0;

typedef enum en_CS0		// Timer/Counter0 Clock Select Flags
{
	CS0_NO_CLOCK = 0,
	CS0_1,
	CS0_8,
	CS0_64,
	CS0_256,
	CS0_1024,
	CS0_XTRN_FALL,
	CS0_XTRN_RISE
	
} en_CS0 ;

/* Timer/Counter 2 */
typedef enum en_WGM2	// Timer/Counter2 Wave Generation Flags
{
	WGM2_NORMAL = 0,
	WGM2_CTC,
	WGM2_FAST_PWM,
	WGM2_PHASE_CORRECT_PWM,
	
} en_WGM2;

typedef enum en_COM2	// Timer/Counter2 Compare Output Mode Flags
{
	COM2_DISCONNECTED = 0,
	
	COM2_NON_PWM_TOGGLE,
	COM2_NON_PWM_CLEAR,
	COM2_NON_PWM_SET,
	
	COM2_FAST_PWM_TOGGLE,
	COM2_FAST_PWM_NV,		// NON-INVERTING
	COM2_FAST_PWM_IV,		// INVERTING
	
	COM2_PHASE_CORRECT_TOGGLE,
	COM2_PHASE_CORRECT_NV,
	COM2_PHASE_CORRECT_IV
	
} en_COM2;

typedef enum en_CS2		// Timer/Counter2 Clock Select Flags
{
	CS2_NO_CLOCK = 0,
	CS2_1,
	CS2_8,
	CS2_32,
	CS2_64,
	CS2_128,
	CS2_256,
	CS2_1024
	
} en_CS2 ;

/* Timer/Counter 1 */
typedef enum en_WGM1	// Timer/Counter1 Wave Generation Flags
{
	WGM1_NORMAL = 0,
	WGM1_CTC,
	WGM1_FAST_PWM,
	WGM1_PHASE_CORRECT_PWM,
	
} en_WGM1;

typedef enum en_COM1	// Timer/Counter1 Compare Output Mode
{
	COM1_DISCONNECTED = 0,
	
	COM1_NON_PWM_TOGGLE,
	COM1_NON_PWM_CLEAR,
	COM1_NON_PWM_SET,
	
	COM1_FAST_PWM_TOGGLE,
	COM1_FAST_PWM_NV,		// NON-INVERTING
	COM1_FAST_PWM_IV,		// INVERTING
	
	COM1_PHASE_CORRECT_TOGGLE,
	COM1_PHASE_CORRECT_NV,
	COM1_PHASE_CORRECT_IV
	
} en_COM1;

typedef enum en_CS1		// Timer/Counter1 Clock Select Flags
{
	CS1_NO_CLOCK = 0,
	CS1_1,
	CS1_8,
	CS1_64,
	CS1_256,
	CS1_1024,
	CS1_XTRN_FALL,
	CS1_XTRN_RISE
	
} en_CS1;

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
//	Function Declarations
////////////////////////////////////////////////////////////////////

// timer 0
/*
* Initializes Timer/Counter0 by setting the WGM and the COM.
*
* @param[in]	_WGM	The wave generation flag.
* @param[in]	_COM	The compare output mode flag.
* @param[in]	_setInterrupt	Enable interrupts or not.
* @return		TIM_SUCCESS on success, TIM_FAILURE otherwise
*/
TIM_STATE Timer0_Init(en_WGM0 _WGM, en_COM0 _COM, uint8_t _setInterrupt);

/*
* A one-call function that initializes the timer and blocks
* the calling thread until the specified duration elapses..
*
* @param[in]	_duration	The sleep duration in milli-seconds.
* @param[in]	_clk		The clock select flag/prescaler.
* @return		TIM_SUCCESS on success, TIM_FAILURE otherwise
*/
TIM_STATE Timer0_Sleep(uint16_t _duration, en_CS0 _clk);

/*
* Starts an initialized timer. Sets the Output Compare Register 
* and the clock.
*
* @param[in]	_clk		The clock select flag/prescaler.
* @param[in]	_count		The number of ticks to count.
* @return		TIM_SUCCESS on success, TIM_FAILURE otherwise
*/
TIM_STATE Timer0_Start(en_CS0 _clk, uint16_t _count);

/*
* Stops the timer by clearing the clock select bits.
* @return		TIM_SUCCESS on success, TIM_FAILURE otherwise
*/
TIM_STATE Timer0_Stop();

//// timer 2
/*
* Initializes Timer/Counter2 by setting the WGM and the COM.
*
* @param[in]	_WGM	The wave generation flag.
* @param[in]	_COM	The compare output mode flag.
* @param[in]	_setInterrupt	Enable interrupts or not.
* @return		TIM_SUCCESS on success, TIM_FAILURE otherwise
*/
TIM_STATE Timer2_Init(en_WGM2 _WGM, en_COM2 _COM, uint8_t _setInterrupt);

/*
* A one-call function that initializes the timer and blocks
* the calling thread until the specified duration elapses.
*
* @param[in]	_duration	The sleep duration in milli-seconds.
* @param[in]	_clk		The clock select flag/prescaler.
* @return		TIM_SUCCESS on success, TIM_FAILURE otherwise
*/
TIM_STATE Timer2_Sleep(uint16_t _duration, en_CS2 _clk);

/*
* Starts an initialized timer. Sets the Output Compare Register
* and the clock.
*
* @param[in]	_clk		The clock select flag/prescaler.
* @param[in]	_count		The number of ticks to count.
* @return		TIM_SUCCESS on success, TIM_FAILURE otherwise
*/
TIM_STATE Timer2_Start(en_CS2 _clk, uint16_t _count);

/*
* Stops the timer by clearing the clock select bits.
* @return		TIM_SUCCESS on success, TIM_FAILURE otherwise
*/
TIM_STATE Timer2_Stop();

// timer 1
/* Note: 
	Timer 1 is more complex since there are 2 output compare units 
	and a lot of different operating modes.
	For now it will only implement the same logic as Timers 0 & 2 
	with the difference of increased resolution.
	-- ONLY OCR1A IS USED --
*/

/*
* Initializes Timer/Counter1 by setting the WGM and the COM.
*
* @param[in]	_WGM	The wave generation flag.
* @param[in]	_COM	The compare output mode flag.
* @param[in]	_setInterrupt	Enable interrupts or not.
* @return		TIM_SUCCESS on success, TIM_FAILURE otherwise
*/
TIM_STATE Timer1_Init(en_WGM1 _WGM, en_COM1 _COM, uint8_t _setInterrupt);

/*
* A one-call function that initializes the timer and blocks
* the calling thread until the specified duration elapses.
*
* @param[in]	_duration	The sleep duration in milli-seconds.
* @param[in]	_clk		The clock select flag/prescaler.
* @return		TIM_SUCCESS on success, TIM_FAILURE otherwise
*/
TIM_STATE Timer1_Sleep(uint16_t _duration, en_CS1 _clk);

/*
* Starts an initialized timer. Sets the Output Compare Register
* and the clock.
*
* @param[in]	_clk		The clock select flag/prescaler.
* @param[in]	_count		The number of ticks to count.
* @return		TIM_SUCCESS on success, TIM_FAILURE otherwise
*/
TIM_STATE Timer1_Start(en_CS1 _clk, uint16_t _count);

/*
* Stops the timer by clearing the clock select bits.
* @return		TIM_SUCCESS on success, TIM_FAILURE otherwise
*/
TIM_STATE Timer1_Stop();


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

#endif /* TIMER_H_ */