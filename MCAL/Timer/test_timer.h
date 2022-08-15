/* -------------------------------------
* Author: Mahmoud Abdullah
* EgFWD Professional Embedded Systems
* Github: https://github.com/Th3-0b3l1sk
* --------------------------------------
*/

#ifndef TEST_TIMER_H_
#define TEST_TIMER_H_

#include "./timer.h"
/* what should the timer tests include? damn that can take forever to test */

////////////////////////////////////////////////////////////////////
//	Test Functions
////////////////////////////////////////////////////////////////////

TIM_STATE test_Timer0_Init(en_WGM0 _WGM, en_COM0 _COM, uint8_t _setInterrupt)
{
	return Timer0_Init(_WGM, _COM, _setInterrupt);
}
TIM_STATE test_Timer0_Sleep(uint16_t _duration, en_CS0 _clk)
{
	return Timer0_Sleep(_duration, _clk);
}
TIM_STATE test_Timer0_Start(en_CS0 _clk, uint16_t _count)
{
	return Timer0_Start(_clk, _count);
}
TIM_STATE test_Timer0_Stop()
{
	return Timer0_Stop();
}
TIM_STATE test_Timer2_Init(en_WGM2 _WGM, en_COM2 _COM, uint8_t _setInterrupt)
{
	return Timer2_Init(_WGM, _COM, _setInterrupt);
}
TIM_STATE test_Timer2_Sleep(uint16_t _duration, en_CS2 _clk)
{
	return Timer2_Sleep(_duration, _clk);
}
TIM_STATE test_Timer2_Start(en_CS2 _clk, uint16_t _count)
{
	return Timer2_Start(_clk, _count);
}
TIM_STATE test_Timer2_Stop()
{
	return Timer2_Stop();
}
TIM_STATE test_Timer1_Init(en_WGM1 _WGM, en_COM1 _COM, uint8_t _setInterrupt)
{
	return Timer1_Init(_WGM, _COM, _setInterrupt);
}
TIM_STATE test_Timer1_Sleep(uint16_t _duration, en_CS1 _clk)
{
	Timer1_Sleep(_duration, _clk);
}
TIM_STATE test_Timer1_Start(en_CS1 _clk, uint16_t _count)
{
	return Timer1_Start(_clk, _count);
}
TIM_STATE test_Timer1_Stop()
{
	return Timer1_Stop();
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


#endif /* TEST_TIMER_H_ */