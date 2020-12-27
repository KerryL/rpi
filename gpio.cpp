// File:  gpio.cpp
// Date:  8/30/2013
// Auth:  K. Loux
// Copy:  (c) Copyright 2013
// Desc:  C++ wrapper for Wiring Pi general-purpose I/O function calls.

// Standard C++ headers
#include <cassert>

// Wiring pi headers
#include <wiringPi.h>

// Local headers
#include "gpio.h"

//==========================================================================
// Class:			GPIO
// Function:		None
//
// Description:		Static member initializaiton.
//
// Input Arguments:
//		None
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
bool GPIO::initialized = false;

//==========================================================================
// Class:			GPIO
// Function:		GPIO
//
// Description:		Constructor for GPIO class.
//
// Input Arguments:
//		pin			= const int&, pin number using Wiring Pi numbering scheme.
//					  See:  http://wiringpi.com/pins/
//		direction	= const DataDirection&
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
GPIO::GPIO(const int &pin, const DataDirection &direction) : pin(pin)
{
	if (!initialized)
	{
		wiringPiSetup();
		initialized = true;
	}

	assert(pin >= 0 && pin <= 40);
	SetDataDirection(direction);
}

//==========================================================================
// Class:			GPIO
// Function:		~GPIO
//
// Description:		Destructor for GPIO class.
//
// Input Arguments:
//		None
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
GPIO::~GPIO()
{
	// Turn everything off
	digitalWrite(pin, 0);
	pullUpDnControl(pin, PUD_OFF);
}

//==========================================================================
// Class:			GPIO
// Function:		SetDataDirection
//
// Description:		Sets the data direction for the pin.
//
// Input Arguments:
//		direction	= const DataDirection&
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
void GPIO::SetDataDirection(const DataDirection &direction)
{
	assert(direction != DataDirection::PWMOutput || pin == 1);

	if (direction == DataDirection::Output)
		SetPullUpDown(PullResistance::Off);

	this->direction = direction;

	if (direction == DataDirection::Input)
		pinMode(pin, INPUT);
	else if (direction == DataDirection::Output)
		pinMode(pin, OUTPUT);
	else if (direction == DataDirection::PWMOutput)
		pinMode(pin, PWM_OUTPUT);
	else
		assert(false);
}

//==========================================================================
// Class:			GPIO
// Function:		SetPullUpDown
//
// Description:		Sets the state of the optional internal pull-up/pull-down resistors.
//
// Input Arguments:
//		state	= const PullResistance&
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
void GPIO::SetPullUpDown(const PullResistance &state)
{
	assert(state == PullResistance::Off || direction == DataDirection::Input);

	if (state == PullResistance::Off)
		pullUpDnControl(pin, PUD_OFF);
	else if (state == PullResistance::PullUp)
		pullUpDnControl(pin, PUD_UP);
	else if (state == PullResistance::PullDown)
		pullUpDnControl(pin, PUD_DOWN);
	else
		assert(false);
}

//==========================================================================
// Class:			GPIO
// Function:		SetOutput
//
// Description:		Sets the state of the output pin.
//
// Input Arguments:
//		high	= const bool&, true for high, false for low
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
void GPIO::SetOutput(const bool &high)
{
	assert(direction == DataDirection::Output);

	digitalWrite(pin, high ? 1 : 0);
}

//==========================================================================
// Class:			GPIO
// Function:		GetInput
//
// Description:		Reads the status of the input pin.
//
// Input Arguments:
//		None
//
// Output Arguments:
//		None
//
// Return Value:
//		bool, true for high, false otherwise
//
//==========================================================================
bool GPIO::GetInput()
{
	assert(direction == DataDirection::Input);
	return digitalRead(pin) == 1;
}
