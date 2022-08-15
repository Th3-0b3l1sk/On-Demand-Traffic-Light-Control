/* -------------------------------------
* Author: Mahmoud Abdullah
* EgFWD Professional Embedded Systems
* Github: https://github.com/Th3-0b3l1sk
* --------------------------------------
*/

#ifndef TEST_GPIO_H_
#define TEST_GPIO_H_

#include "./gpio.h"

////////////////////////////////////////////////////////////////////
//	Test Functions
////////////////////////////////////////////////////////////////////

GPIO_STATE	test_IO_PortInit	(const IO_Port _port, IO_Dir _dir)
{
	return IO_PortInit(_port, _dir);
}
GPIO_STATE	test_IO_PortWrite	(const IO_Port _port, const uint8_t _value)
{
	return IO_PortWrite(_port, _value);
}
uint8_t		test_IO_PortRead	(const IO_Port _port, uint8_t* _value)
{
	return IO_PortRead(_port, _value);
}
GPIO_STATE	test_IO_PortToggle	(const IO_Port _port)
{
	return IO_PortToggle(_port);
}
GPIO_STATE	test_IO_PinInit		(const IO_Port _port, const uint8_t _pin, IO_Dir _dir)
{
	return IO_PinInit(_port, _pin, _dir);
}
GPIO_STATE	test_IO_PinWrite	(const IO_Port _port, const uint8_t _pin, const uint8_t _value)
{
	return IO_PinWrite(_port, _pin, _value);
}
uint8_t		test_IO_PinRead		(const IO_Port _port, const uint8_t _pin, uint8_t* _value)
{
	return IO_PinRead(_port, _pin, _value);
}
GPIO_STATE	test_IO_PinToggle	(const IO_Port _port, const uint8_t _pin)
{
	return IO_PinToggle(_port, _pin);
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

#endif /* TEST_GPIO_H_ */