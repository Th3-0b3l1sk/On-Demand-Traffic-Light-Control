/* -------------------------------------
* Author: Mahmoud Abdullah
* EgFWD Professional Embedded Systems
* Github: https://github.com/Th3-0b3l1sk
* --------------------------------------
*/

#include "./interrupt.h"


////////////////////////////////////////////////////////////////////
//	Function Definitions
////////////////////////////////////////////////////////////////////

void  INT_ENABLE_ALL(void)
{
	__asm__ __volatile__ ("sei" ::: "memory");
	
	
}

void  INT_DISABLE_ALL(void)
{
	__asm__ __volatile__ ("cli" ::: "memory");
	
	
}

INT_STATE INT0_Init(en_ISC0 _ISC0)
{
	BIT_CLEAR(DDRD, PIN_2);
	BIT_WRITE(GICR, INT0);
	BIT_MASK_WRITE(MCUCR,  _ISC0);
	
	return INT_SUCCESS;
}

INT_STATE INT1_Init(en_ISC1 _ISC1)
{
	BIT_CLEAR(DDRD, PIN_3);
	BIT_WRITE(GICR, INT1);
	BIT_MASK_WRITE(MCUCR, _ISC1 << 2);
	
	return INT_SUCCESS;
}

INT_STATE INT2_Init(en_ISC2 _ISC2)
{
	BIT_CLEAR(DDRB, PIN_2);
	BIT_WRITE(GICR, INT2);
	BIT_MASK_WRITE(MCUCSR, _ISC2 << ISC20 );
	
	return INT_SUCCESS;
}

INT_STATE INT0_Disable(void)
{
	BIT_CLEAR(GICR, INT0);

	return INT_SUCCESS;
}

INT_STATE INT1_Disable(void)
{
	BIT_CLEAR(GICR, INT1);
	
	return INT_SUCCESS;
}

INT_STATE INT2_Disable(void)
{
	BIT_CLEAR(GICR, INT2);
	
	return INT_SUCCESS;
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////