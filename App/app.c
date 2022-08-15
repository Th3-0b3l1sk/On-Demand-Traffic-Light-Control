/* -------------------------------------
* Author: Mahmoud Abdullah
* EgFWD Professional Embedded Systems
* Github: https://github.com/Th3-0b3l1sk
* --------------------------------------
*/

#include "./app.h"

#define F_CPU 1000000

#include "../MCAL/GPIO/gpio.h"
#include "../MCAL/Timer/timer.h"
#include "../MCAL/Interrupt/interrupt.h"
#include "../ECUAL/Button/button.h"
#include "../ECUAL/Led/led.h"

////////////////////////////////////////////////////////////////////
//	Macros
////////////////////////////////////////////////////////////////////

#define SHORT_PRESS 250	// 250ms
#define MAX_OVS		3	// 3 * 250ms = 750ms

#define LED_RED	0
#define LED_YEL	1
#define LED_GRN	2

/*
*	The flag IN_INT_PROCESSING switches between two operating modes:
*		1) All pedestrian processing logic is handled in the interrupt handler
*	and the uC is reset after the interrupt finishes. Transitioning from normal
*	to pedestrian mode is instantaneous.
*		2) The interrupt sets a global flag and returns immediately. the normal
*	mode logic checks whether this flag is set and if so switches to pedestrian
*	mode AFTER the current 'state' is completed. This behavior can be modified
*	to be instantaneous like mode 1.
*/
#define IN_INT_PROCESSING 0

#define SEC(x)	x * 1000

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
//	Global objects
////////////////////////////////////////////////////////////////////

st_Led carGreen  = { .port = PORT_A, .pin = PIN_0};		// The Car's Green Light
st_Led carYellow = { .port = PORT_A, .pin = PIN_1};		// The Car's Yellow Light
st_Led carRed    = { .port = PORT_A, .pin = PIN_2};		// The Car's Red light

st_Led pedGreen  = { .port = PORT_B, .pin = PIN_0};		// Pedestrian's Green Light
st_Led pedYellow = { .port = PORT_B, .pin = PIN_1};		// Pedestrian's Yellow Light
st_Led pedRed    = { .port = PORT_B, .pin = PIN_2};		// Pedestrian's Red Light

//st_Led debug = {.port = PORT_C, .pin = PIN_0};	// debug led
	
uint8_t g_CurrentLed = 0;	// global flag marks the current 'state' in the normal mode.

// Detect long/short presses
uint8_t g_Pressed = 0;		// whether the pedestrian button was/is pressed
uint8_t g_Overflows = 0;	// number of overflows required for the long press detection	
uint8_t g_IsPedestrian = 0;	// pedestrian global flag

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
//	Function Definitions
////////////////////////////////////////////////////////////////////

/* 
* Tailored specifically for this application by setting 2 LEDs in the API
* each blink is hard coded for 500ms for the blinking purpose. _seconds is
* the number of seconds to blink.
*/
void Blink(st_Led* _led1, st_Led* _led2, uint8_t _seconds)
{
	Timer1_Init(WGM1_CTC, COM1_DISCONNECTED, 0);
	
	for(int i = 0; i < _seconds * 2; i++){
		LED_Toggle(_led1);
		LED_Toggle(_led2);
		Timer1_Start(CS1_8, 62500);	// 500ms
		
		while(!IS_SET(TIFR, OCF1A))
			;
		BIT_WRITE(TIFR, OCF1A);
	}
	
	Timer1_Stop();
}

// The termination sequence of both states in the Pedestrian Mode
void inline TerminationSequence()
{
	LED_Off(&carRed);
	LED_On(&pedGreen);
	Blink(&carYellow, &pedYellow, 5);	// blinks for 5 seconds. BLOCKS
	LED_Off(&pedGreen);
	
	LED_On(&pedRed);
	LED_On(&carGreen);
	Timer0_Sleep(SEC(5), CS0_1024);
	LED_Off(&carGreen);
	LED_Off(&pedRed);
}

void PedestrianMode()
{
	switch(g_CurrentLed)
	{
		case LED_RED:
		{
			LED_On(&carRed);
			LED_On(&pedGreen);
			Timer0_Sleep(SEC(5), CS0_1024);
			LED_Off(&carRed);
			LED_Off(&pedGreen);

			TerminationSequence();
			break;
		}

		case LED_YEL:
		case LED_GRN:
		{
			LED_Off(&carYellow);
			LED_On(&pedRed);
			Blink(&carYellow, &pedYellow, 5);
			LED_Off(&carGreen);
			LED_Off(&pedRed);
			
			LED_On(&carRed);
			LED_On(&pedGreen);
			Timer0_Sleep(SEC(5), CS0_1024);
			
			TerminationSequence();
			break;
			
		}
		
	}
}

/* 
* The normal mode of the traffic lights 
* Green -> Yellow -> Red -> Yellow -> Green
*/
void NormalMode()
{
#if IN_INT_PROCESSING
	// green
	g_CurrentLed = LED_GRN;
	LED_On(&carGreen);
	Timer0_Sleep(SEC(5), CS0_1024);
	LED_Off(&carGreen);
	
	// yellow
	g_CurrentLed = LED_YEL;
	Blink(&carYellow, NULL,  5);	// blinks for 5 seconds. BLOCKS
	
	// red
	g_CurrentLed = LED_RED;
	LED_On(&carRed);
	Timer0_Sleep(SEC(5), CS0_1024);
	LED_Off(&carRed);
	
	// yellow
	g_CurrentLed = LED_YEL;
	Blink(&carYellow, NULL, 5);		// blinks for 5 seconds. BLOCKS

#else
	// green
	g_CurrentLed = LED_GRN;
	LED_On(&carGreen);
	Timer0_Sleep(SEC(5), CS0_1024);
	LED_Off(&carGreen);
	if(g_IsPedestrian) goto pedestrian_handler;
	
	// yellow
	g_CurrentLed = LED_YEL;
	Blink(&carYellow, NULL,  5);	// blinks for 5 seconds. BLOCKS
	if(g_IsPedestrian) goto pedestrian_handler;
	
	// red
	g_CurrentLed = LED_RED;
	LED_On(&carRed);
	Timer0_Sleep(SEC(5), CS0_1024);
	LED_Off(&carRed);
	if(g_IsPedestrian) goto pedestrian_handler;
	
	// yellow
	g_CurrentLed = LED_YEL;
	Blink(&carYellow, NULL, 5);		// blinks for 5 seconds. BLOCKS

pedestrian_handler:
	if(g_IsPedestrian){
		PedestrianMode();
		g_IsPedestrian = 0;
	}
#endif
}

/* USED WHEN HANDLING THE PEDESTRIAN LOGIC IN THE INTERRUPT HANDLER 
* The Interrupt Service Routine of Timer2. 
* Detects long/short presses.  **/
ISR(VEC_TIMER2_COMP)
{
	//LED_Toggle(&debug);
	
	g_Overflows += 1;
	
	Timer2_Start(CS2_1024, SHORT_PRESS);
	BIT_WRITE(TIFR, OCF2);
}

/* The push button Interrupt Service Handler.
* Used to switch to Pedestrian mode from Normal mode. */
ISR(VEC_INT0)
{
#if IN_INT_PROCESSING
	// the button is pushed
	if ( g_Pressed == 0 ) {
		
		//LED_On(&debug);
		g_Pressed++;
		
		/* 
		* Enabling interrupts as Timer2 is set for 250ms which might
		* End before returning from the interrupt, thus the interrupt 
		* will be ignored and the Interrupt Handler won't be executed.
		*/
		INT_ENABLE_ALL();
		
		Timer2_Init(WGM2_CTC, COM2_NON_PWM_TOGGLE, 1);
		Timer2_Start(CS2_1024, SHORT_PRESS);
		
		return;
	
	// the button is released
	} else if ( g_Pressed == 1) {
		
		Timer2_Stop();
		g_Pressed = 0;
		
		if(g_Overflows > MAX_OVS){	// each overflow is 250ms thus a short press <= 750ms
			g_Overflows = 0;
			return;
		}
		
		g_Overflows = 0;
	}

	Timer0_Stop();	// The interrupt might fire while Timer0 is being used.
	
	/* handle the interrupt logic here */
	
	PedestrianMode();

	/* 
	* Resetting the uC to restart the NormalMode.
	* Jumping to the reset vector doesn't clear the pins and
	* thus needs to be cleared manually.
	 */
	IO_PortWrite(PORT_A, 0x00);
	IO_PortWrite(PORT_B, 0x00);
	
	__asm__ __volatile__ ("jmp 0x00");
#else

	if(g_IsPedestrian == 1)
		return;
		
	g_IsPedestrian = 1;
#endif
}


APP_STATE AppRun(void)
{
	
	INT_ENABLE_ALL();
	
	/* 
	* To detect long/short presses. The push button is pulled high.
	* when the button is pushed, the button pin will be connected to 
	* the ground and the interrupt is fired. When the button is released
	* the button pin is pulled high and another interrupt is fired.
	* thus the press duration can be detected between the two level changes.
	*/
	INT0_Init(ISC0_ANY_CHANGE);		
	
	// Initialize the car's LEDs
	LED_Init(&carRed);	LED_Init(&carYellow);	LED_Init(&carGreen);
	// Initialize the pedestrian's LEDs
	LED_Init(&pedRed);	LED_Init(&pedYellow);	LED_Init(&pedGreen);
	
	//LED_Init(&debug);	// Debug Led
	
	// Initialize the Pedestrian's push button
	BUTTON_Init(PORT_D, PIN_2);

	//////////////////////////////////////////////////////////////////////////////
	/*							THE MAIN LOOP									*/ 
	//////////////////////////////////////////////////////////////////////////////
	
	while(1)
	{
		/* switching to pedestrian mode is done by the interrupt */
		NormalMode();	
	}
	
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	
	return APP_SUCCESS;
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////