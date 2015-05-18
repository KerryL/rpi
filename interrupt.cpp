// File:  interrupt.cpp
// Date:  5/18/2015
// Auth:  K. Loux
// Copy:  (c) Copyright 2015
// Desc:  C++ wrapper for Wiring Pi interrupt functions.

// Standard C++ headers
#include <cassert>

// Wiring pi headers
#include <wiringPi.h>

// rpi headers
#include "interrupt.h"

//==========================================================================
// Class:			Interrupt
// Function:		Interrupt
//
// Description:		Constructor for Interrupt class.
//
// Input Arguments:
//		pin		= const int&, pin number using Wiring Pi numbering scheme.
//				  See:  http://wiringpi.com/pins/
//		isr		= const InterruptServiceRoutine
//		edge	= const EdgeDirection&
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
Interrupt::Interrupt(const int &pin, const InterruptServiceRoutine isr,
	const EdgeDirection &edge) : isr(isr)
{
	int edgeFlag;
	if (edge == EdgeRising)
		edgeFlag = INT_EDGE_RISING;
	else if (edge == EdgeFalling)
		edgeFlag = INT_EDGE_FALLING;
	else if (edge == EdgeBoth)
		edgeFlag = INT_EDGE_BOTH;
	else if (edge == EdgePreconfigured)
		edgeFlag = INT_EDGE_SETUP;
	else
		assert(false);

	wiringPiISR(pin, edge, isr);// TODO:  What do we expect this to return?
}