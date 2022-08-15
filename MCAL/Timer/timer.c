/* -------------------------------------
* Author: Mahmoud Abdullah
* EgFWD Professional Embedded Systems
* Github: https://github.com/Th3-0b3l1sk
* --------------------------------------
*/

#include "./timer.h"
#include <math.h>	// for ceil

#ifndef F_CPU
#define F_CPU 1000000
#endif

////////////////////////////////////////////////////////////////////
//	Type Definitions
////////////////////////////////////////////////////////////////////

typedef enum en_Timer
{
	TIMER_0,
	TIMER_1,
	TIMER_2
	
} en_Timer;

typedef struct st_TimerState
{
	uint8_t initialized;
	uint8_t WGM;
	uint8_t COM;
	
}	st_TimerState;

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////
//	Static Global Objects
////////////////////////////////////////////////////////////////////

/* NOT THREAD SAFE*/
// keeps track of the 3 timers GLOBALLY
static st_TimerState g_Timer0 = { 0 };
static st_TimerState g_Timer1 = { 0 };
static st_TimerState g_Timer2 = { 0 };

// the actual prescalers available in the uC. 
// zeros at the end are padding.
static uint16_t prescalers[3][8] =  { { 0, 1, 8, 64, 256, 1024,   0,    0},		// Timer0 Prescalers
									  { 0, 1, 8, 64, 256, 1024,   0,    0},		// Timer1 Prescalers
									  { 0, 1, 8, 32,  64,  128, 256, 1024}		// Timer2 Prescalers
									};

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
//	Function Definitions
////////////////////////////////////////////////////////////////////

static TIM_STATE CalculateSettings(en_Timer _which, uint8_t _clk, uint16_t _duration, uint16_t* _start, uint16_t* _overflows)
{
	uint32_t step_max = 0x0;
	if(_which == TIMER_0 || _which == TIMER_2)
		step_max = MAX_8 + 1;
	else 
		step_max = MAX_16 + 1;
	
	volatile uint16_t prescaler = prescalers[_which][_clk];
	float t_tick = prescaler / ( F_CPU * 1.0f) * 1000;	// micro to milli
	float t_max = t_tick * step_max;
		
	if(t_max == _duration)
		*_start = _duration;
	else if (_duration < t_max)
		*_start = (t_max - _duration) / t_tick;
	else {
		*_overflows = ceil(_duration / t_max);
		*_start = step_max - ( _duration / t_tick) / *_overflows;
	}
	
	
	return TIM_SUCCESS;
}

// timer 0
TIM_STATE Timer0_Init(en_WGM0 _WGM, en_COM0 _COM, uint8_t _setInterrupt)
{
	if(g_Timer0.initialized == 1) 
		return TIM_FAILURE;
		
	// set the OC0 pin to output mode
	BIT_WRITE(DDRB, PIN_3);	
	
		/* Set Wave Generation Mode */
	switch(_WGM)
	{
		case WGM0_NORMAL:
		{
			// WGM
			BIT_CLEAR(TCCR0, WGM00);
			BIT_CLEAR(TCCR0, WGM01);
			
			BIT_COND_WRITE(TIMSK, TOIE0, _setInterrupt);
			break;
		}
		
		case WGM0_CTC:
		{
			BIT_CLEAR(TCCR0, WGM00);
			BIT_WRITE(TCCR0, WGM01);
			
			BIT_COND_WRITE(TIMSK, OCIE0, _setInterrupt);
			break;
		}
		case WGM0_FAST_PWM:
		{
			BIT_WRITE(TCCR0, WGM00);
			BIT_WRITE(TCCR0, WGM01);
			
			BIT_COND_WRITE(TIMSK, OCIE0, _setInterrupt);
			break;
		}
		case WGM0_PHASE_CORRECT_PWM:
		{
			BIT_WRITE(TCCR0, WGM00);
			BIT_CLEAR(TCCR0, WGM01);
			
			BIT_COND_WRITE(TIMSK, OCIE0, _setInterrupt);
			break;
		}
		default:
			return TIM_FAILURE;
	};
	
		/* Set Output Compare Mode */
	switch(_COM)
	{
		case COM0_NON_PWM_TOGGLE:				// 0 1
		{
			BIT_WRITE(TCCR0, COM00);
			BIT_CLEAR(TCCR0, COM01);
			break;
		}
		
		// intended fall through, all of those cases set the same bits
		case COM0_NON_PWM_CLEAR:				// 1 0
		case COM0_FAST_PWM_NV:
		case COM0_PHASE_CORRECT_NV:
		{
			BIT_CLEAR(TCCR0, COM00);
			BIT_WRITE(TCCR0, COM01);
			break;
		}
		
		case COM0_NON_PWM_SET:					// 1 1
		case COM0_FAST_PWM_IV:
		case COM0_PHASE_CORRECT_IV:
		{
			BIT_WRITE(TCCR0, COM00);
			BIT_WRITE(TCCR0, COM01);
			break;
		}
		
		default:
			return TIM_FAILURE;
	}
	
	g_Timer0.initialized = 1;
	g_Timer0.COM = _COM;
	g_Timer0.WGM = _WGM;
	
	return TIM_SUCCESS;
}

TIM_STATE Timer0_Start(en_CS0 _clk, uint16_t _count)
{
	if(_clk > CS0_XTRN_RISE || g_Timer0.initialized == 0)
		return TIM_FAILURE;
		
	switch(g_Timer0.WGM) 
	{
		case WGM0_NORMAL:
		{
			TCNT0 = _count & 0xFF;
			break;
		}
		case WGM0_PHASE_CORRECT_PWM:
		case WGM0_FAST_PWM:
		case WGM0_CTC:
		{
			OCR0 = _count & 0xFF;
			break;
		}
		
		default:
			return TIM_FAILURE;
	}
	
	TCCR0 |= (_clk & 0x07);
	
	return TIM_SUCCESS;
}

TIM_STATE Timer0_Sleep(uint16_t _duration, en_CS0 _clk)
{
	if(_clk > CS0_XTRN_RISE)
		return TIM_FAILURE;
		
	Timer0_Init(WGM0_NORMAL, COM0_DISCONNECTED, 0);
	
	uint16_t start = 0x00;
	uint16_t overflows = 0x00;
	
	CalculateSettings(TIMER_0, _clk, _duration, &start, &overflows);
	
	do
	{
		TCNT0 = start;
		TCCR0 |= _clk & 0x07;
		
		while(! IS_SET(TIFR, TOV0))
			;	// busy wait
		BIT_WRITE(TIFR, TOV0);
		
		if(overflows == 0)
			break;
		
	} while ( --overflows);

	Timer0_Stop();
	
	return TIM_SUCCESS;
}

TIM_STATE Timer0_Stop()
{
	TCCR0 = 0x00;
	TCNT0 = 0x00;
	OCR0  = 0x00;
	g_Timer0.initialized = 0;
	
	return TIM_SUCCESS;
}

// timer 2
TIM_STATE Timer2_Init(en_WGM2 _WGM, en_COM2 _COM, uint8_t _setInterrupt)
{
	if(g_Timer2.initialized == 1)
		return TIM_FAILURE;
		
	// set the OC2 pin to output mode
	BIT_WRITE(DDRD, PIN_7);
	
		/* Set Wave Generation Mode */
	switch(_WGM)
	{
		case WGM2_NORMAL:
		{
			BIT_CLEAR(TCCR2, WGM20);
			BIT_CLEAR(TCCR2, WGM21);
			
			BIT_COND_WRITE(TIMSK, TOIE2, _setInterrupt);
			break;
		}
		
		case WGM2_CTC:
		{
			BIT_CLEAR(TCCR2, WGM20);
			BIT_WRITE(TCCR2, WGM21);
			
			BIT_COND_WRITE(TIMSK, OCIE2, _setInterrupt);
			break;
		}
		case WGM2_FAST_PWM:
		{
			BIT_WRITE(TCCR2, WGM20);
			BIT_WRITE(TCCR2, WGM21);
			
			BIT_COND_WRITE(TIMSK, OCIE2, _setInterrupt);
			break;
		}
		case WGM2_PHASE_CORRECT_PWM:
		{
			BIT_WRITE(TCCR2, WGM20);
			BIT_CLEAR(TCCR2, WGM21);
			
			BIT_COND_WRITE(TIMSK, OCIE2, _setInterrupt);
			
			break;
		}
		default:
			return TIM_FAILURE;
	};
	
		/* Set Output Compare Mode */
	switch(_COM)
	{
		case COM2_NON_PWM_TOGGLE:				// 0 1
		{
			BIT_WRITE(TCCR2, COM20);
			BIT_CLEAR(TCCR2, COM21);
			break;
		}
		
		// intended fall through, all of those cases set the same bits
		case COM2_NON_PWM_CLEAR:				// 1 0
		case COM2_FAST_PWM_NV:
		case COM2_PHASE_CORRECT_NV:
		{
			BIT_CLEAR(TCCR2, COM20);
			BIT_WRITE(TCCR2, COM21);
			break;
		}
		case COM2_NON_PWM_SET:					// 1 1
		case COM2_FAST_PWM_IV:
		case COM2_PHASE_CORRECT_IV:
		{
			BIT_WRITE(TCCR2, COM20);
			BIT_WRITE(TCCR2, COM21);
			break;
		}
		
		default:
			return TIM_FAILURE;
	}
	
	g_Timer2.initialized = 1;
	g_Timer2.COM = _COM;
	g_Timer2.WGM = _WGM;
	
	return TIM_SUCCESS;
}

TIM_STATE Timer2_Sleep(uint16_t _duration, en_CS2 _clk)
{
	if(_clk > CS2_1024)
		return TIM_FAILURE;
	
	Timer2_Init(WGM2_NORMAL, COM2_DISCONNECTED, 0);
	
	uint16_t start = 0x00;
	uint16_t overflows = 0x00;
	
	CalculateSettings(TIMER_2, _clk, _duration, &start, &overflows);
	
	do
	{
		TCNT2 = start;
		TCCR2 |= _clk & 0x07;
		
		while(! IS_SET(TIFR, TOV2))
			;	// busy wait
		BIT_WRITE(TIFR, TOV2);
		
		if (overflows == 0)
			break;
		
	} while ( --overflows);

	Timer2_Stop();
	
	return TIM_SUCCESS;
}

TIM_STATE Timer2_Start(en_CS2 _clk, uint16_t _count)
{
	if(_clk > CS2_1024 || g_Timer2.initialized == 0)
		return TIM_FAILURE;
	
	switch(g_Timer2.WGM)
	{
		case WGM2_NORMAL:
		{
			TCNT2 = _count & 0xFF;
			break;
		}
		case WGM2_PHASE_CORRECT_PWM:
		case WGM2_FAST_PWM:
		case WGM2_CTC:
		{
			OCR2 = _count & 0xFF;
			break;
		}
		
		default:
			return TIM_FAILURE;
	}
	
	TCCR2 |= (_clk & 0x07);

	return TIM_SUCCESS;
}

TIM_STATE Timer2_Stop()
{
	TCCR2 = 0x00;
	TCNT2 = 0x00;
	g_Timer2.initialized = 0;
	
	return TIM_SUCCESS;
}

// timer 1 ( 16-bit )
TIM_STATE Timer1_Init(en_WGM1 _WGM, en_COM1 _COM, uint8_t _setInterrupt)
{
	if (g_Timer1.initialized == 1)
		return TIM_FAILURE;
	
	// set the OC1A pin to output mode
	BIT_WRITE(DDRD, PIN_5);
	
		/* Set Wave Generation Mode */
	switch(_WGM)
	{
		case WGM1_NORMAL:				// 0 0 0 0 (0xFFFF = TOP)
		{
			BIT_CLEAR(TCCR1A, WGM10);
			BIT_CLEAR(TCCR1A, WGM11);
			BIT_CLEAR(TCCR1B, WGM12);
			BIT_CLEAR(TCCR1B, WGM13);
			
			BIT_COND_WRITE(TIMSK, TOIE1, _setInterrupt);
			break;
		}
		
		case WGM1_CTC:					// 0 1 0 0 ( OCR1A = TOP)
		{
			BIT_CLEAR(TCCR1A, WGM10);
			BIT_CLEAR(TCCR1A, WGM11);
			BIT_WRITE(TCCR1B, WGM12);
			BIT_CLEAR(TCCR1B, WGM13);
			
			BIT_COND_WRITE(TIMSK, OCIE1A, _setInterrupt);
			
			break;
		}
		
		case WGM1_FAST_PWM:				// 1 1 1 0 ( ICR1A = TOP )
		{
			BIT_WRITE(TCCR1A, WGM10);
			BIT_WRITE(TCCR1A, WGM11);
			BIT_WRITE(TCCR1B, WGM12);
			BIT_WRITE(TCCR1B, WGM13);
			
			BIT_COND_WRITE(TIMSK, OCIE1A, _setInterrupt);
			
			break;
		}
		
		case WGM1_PHASE_CORRECT_PWM:	// 1 0 1 1 ( OCR1A = TOP )
		{
			BIT_WRITE(TCCR1A, WGM10);
			BIT_WRITE(TCCR1A, WGM11);
			BIT_CLEAR(TCCR1B, WGM12);
			BIT_WRITE(TCCR1B, WGM13);
			
			BIT_COND_WRITE(TIMSK, OCIE1A, _setInterrupt);
			
			break;
			
		}
		default:
			return TIM_FAILURE;	
	}
	
		/* Set Output Compare Mode (ONLY OCRA)*/
	switch (_COM)
	{
		case COM1_DISCONNECTED:			// 0 0
		{
			BIT_CLEAR(TCCR1A, COM1A0);
			BIT_CLEAR(TCCR1A, COM1A1);
			
			break;
		}
		
		case COM1_NON_PWM_TOGGLE:		// 0 1
		case COM1_FAST_PWM_TOGGLE:
		case COM1_PHASE_CORRECT_TOGGLE:
		{
			if(_WGM == WGM1_FAST_PWM || 
			   _WGM == WGM1_PHASE_CORRECT_PWM || 
			   _WGM == WGM1_NORMAL) {
				   
				BIT_WRITE(TCCR1A, COM1A0);
				BIT_CLEAR(TCCR1A, COM1A1);
			} else {
				BIT_CLEAR(TCCR1A, COM1A0);
				BIT_CLEAR(TCCR1A, COM1A1);
			}
			
			break;
		}
		
		case COM1_NON_PWM_CLEAR:		// 1 0
		case COM1_FAST_PWM_NV:
		case COM1_PHASE_CORRECT_NV:
		{
			BIT_WRITE(TCCR1A, COM1A0);
			BIT_CLEAR(TCCR1A, COM1A1);
			
			break;
		}
		
		case COM1_NON_PWM_SET:			// 1 1
		case COM1_FAST_PWM_IV:
		case COM1_PHASE_CORRECT_IV:
		{
			BIT_WRITE(TCCR1A, COM1A0);
			BIT_WRITE(TCCR1A, COM1A1);
			
			break;
		}
		
		default:
			return TIM_FAILURE;		
				
	}
	
	g_Timer1.initialized = 1;
	g_Timer1.COM = _COM;
	g_Timer1.WGM = _WGM;
	
	return TIM_SUCCESS;
}

TIM_STATE Timer1_Sleep(uint16_t _duration, en_CS1 _clk)
{
	/* Note: there might be problems if this function is interrupted 
	 while updating the 16bit registers TCCR1 & TCCR1B
	*/	
	
	if(_clk > CS1_1024)
		return TIM_FAILURE;
	
	Timer1_Init(WGM1_NORMAL, COM1_DISCONNECTED, 0);
	
	
	uint16_t start = 0x00;
	uint16_t overflows = 0x00;
	
	CalculateSettings(TIMER_1, _clk, _duration, &start, &overflows);
	do
	{
		
		TCNT1H = start >> 8;
		TCNT1L = start & 0xFF;
		
		
		TCCR1B |= ( _clk & 0x07 );
		
		while( ! IS_SET(TIFR, TOV1))
			;
		BIT_WRITE(TIFR, TOV1);
		
		
		if(overflows == 0)
			break;
		
	} while ( --overflows);

	Timer1_Stop();
	
	return TIM_SUCCESS;
}

TIM_STATE Timer1_Start(en_CS1 _clk, uint16_t _count)
{
    /* Note: there might be problems if this function is interrupted 
	 while updating the 16bit register OCR1A
	*/	
	if(_clk > CS1_XTRN_RISE || g_Timer1.initialized == 0)
		return TIM_FAILURE;
		

	switch(g_Timer1.WGM)
	{
		case WGM1_NORMAL:
		{
			TCNT1H = _count >> 8;
			TCNT1L = _count & 0xFF;
			break;
		}
		case WGM1_PHASE_CORRECT_PWM:
		case WGM1_FAST_PWM:
		case WGM1_CTC:
		{
			//ICR1H = (_count & 0xFF00) >> 8;
			//ICR1L = (_count & 0x00FF);
			
			OCR1AH = (_count & 0xFF00) >> 8;
			OCR1AL = (_count & 0x00FF);
			break;
		}
		
		default:
			return TIM_FAILURE;
	}
	
	
	TCCR1B |= ( _clk & 0x07 );
	
	return TIM_SUCCESS;
}

TIM_STATE Timer1_Stop()
{
	TCCR1A  = 0x00;
	TCNT1H  = 0x00;
	TCNT1L  = 0x00;
	g_Timer1.initialized = 0;
	
	return TIM_SUCCESS;
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////