/* -------------------------------------
* Author: Mahmoud Abdullah
* EgFWD Professional Embedded Systems
* Github: https://github.com/Th3-0b3l1sk
* --------------------------------------
*/

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "../../Util/bits.h"
#include "../../Util/defs.h"
#include "../../MCAL/GPIO/gpio.h"

////////////////////////////////////////////////////////////////////
//	Macros
////////////////////////////////////////////////////////////////////

#define MCUCR	*((volatile uint8_t*) 0x55)		// MCU Control Register
#define MCUCSR	*((volatile uint8_t*) 0x54)		// MCU Control and Status Register
#define GICR	*((volatile uint8_t*) 0x5B)		// General Interrupt Control Register
#define GIFR	*((volatile uint8_t*) 0x5A)		// General Interrupt Flag Register

#define INT0	BIT_6	// Interrupt 0
#define INT1	BIT_7	// Interrupt 1
#define INT2	BIT_5	// Interrupt 2

#define ISC00	BIT_0	// Interrupt Sense Control 00
#define ISC01	BIT_1	// Interrupt Sense Control 01
#define ISC10	BIT_3	// Interrupt Sense Control 10	
#define ISC11	BIT_4	// Interrupt Sense Control 11
#define ISC20	BIT_6	// Interrupt Sense Control 20	

#define VEC_RESET			__vector_0		// External Pin, Power-on Reset, Brown-out Reset, Watchdog Reset, and JTAG AVR Reset
#define VEC_INT0			__vector_1		// External Interrupt Request 0
#define VEC_INT1			__vector_2		// External Interrupt Request 1
#define VEC_INT2			__vector_3		// External Interrupt Request 2
#define VEC_TIMER2_COMP		__vector_4		// Timer/Counter2 Compare Match
#define VEC_TIMER2_OVF		__vector_5		// Timer/Counter2 Overflow
#define VEC_TIMER1_CAPT		__vector_6		// Timer/Counter1 Capture Event
#define VEC_TIMER1_COMPA	__vector_7		// Timer/Counter1 Compare Match A
#define VEC_TIMER1_COMPB	__vector_8		// Timer/Counter1 Compare Match B
#define VEC_TIMER1_OVF		__vector_9		// Timer/Counter1 Overflow
#define VEC_TIMER0_COMP		__vector_10		// Timer/Counter0 Compare Match
#define VEC_TIMER0_OVF		__vector_11		// Timer/Counter0 Overflow
#define VEC_SPI_STC			__vector_12		// Serial Transfer Complete
#define VEC_USART_RXC		__vector_13		// USART, Rx Complete
#define VEC_USART_UDRE		__vector_14		// USART Data Register Empty
#define VEC_USART_TXC		__vector_15		// USART, Tx Complete
#define VEC_ADC				__vector_16		// ADC Conversion Complete
#define VEC_EE_RDY			__vector_17		// EEPROM Ready
#define VEC_ANA_COMP		__vector_18		// Analog Comparator
#define VEC_TWI				__vector_19		// Two-wire Serial Interface
#define VEC_SPM_RDY			__vector_20		// Store Program Memory Ready

// Interrupt Service Routine
#define ISR(handler)	void handler(void) __attribute__((signal, used));	\	
						void handler(void) 

#define INT_READY(which)	BIT_READ(GIFR, which)
#define INT_CLEAR(which)	BIT_CLEAR(GIFR, which)

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
//	Type Definitions
////////////////////////////////////////////////////////////////////

typedef enum INT_STATE
{
	INT_SUCCESS = 1,
	INT_FAILURE = 0
	
} INT_STATE;


typedef enum en_ISC0
{
	ISC0_LOW_LEVEL = 0,
	ISC0_ANY_CHANGE,
	ISC0_FALLING_EDGE,
	ISC0_RISING_EDGE
	
} en_ISC0;  

typedef enum en_ISC1
{
	ISC1_LOW_LEVEL = 0,
	ISC1_ANY_CHANGE,
	ISC1_FALLING_EDGE,
	ISC1_RISING_EDGE
	
} en_ISC1;

typedef enum en_ISC2
{
	ISC2_FALLING_EDGE = 0,
	ISC2_RISING_EDGE
	
} en_ISC2;

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
//	Function Declarations
////////////////////////////////////////////////////////////////////

/*
* Sets the global interrupt flag in SREG.
*/
void inline INT_ENABLE_ALL(void);

/*
* Clears the global interrupt flag in SREG.
*/
void inline INT_DISABLE_ALL(void);

/*
* Initialize interrupt 0
*
* @param[in]	_ISC0	Interrupt sense control 0
* @return		INT_SUCCESS on success, INT_FAILURE otherwise
*/
INT_STATE  INT0_Init(en_ISC0 _ISC0);

/*
* Initialize interrupt 1
*
* @param[in]	_ISC1	Interrupt sense control 1
* @return		INT_SUCCESS on success, INT_FAILURE otherwise
*/
INT_STATE  INT1_Init(en_ISC1 _ISC1);

/*
* Initialize interrupt 2
*
* @param[in]	_ISC2	Interrupt sense control 2
* @return		INT_SUCCESS on success, INT_FAILURE otherwise
*/
INT_STATE  INT2_Init(en_ISC2 _ISC2);

/*
* Disables interrupt 0
*
* @return		INT_SUCCESS on success, INT_FAILURE otherwise
*/
INT_STATE  INT0_Disable(void);

/*
* Disables interrupt 1
*
* @return		INT_SUCCESS on success, INT_FAILURE otherwise
*/
INT_STATE  INT1_Disable(void);

/*
* Disables interrupt 2
*
* @return		INT_SUCCESS on success, INT_FAILURE otherwise
*/
INT_STATE  INT2_Disable(void);

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

#endif /* INTERRUPTS_H_ */