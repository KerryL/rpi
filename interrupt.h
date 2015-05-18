// File:  interrupt.h
// Date:  5/18/2015
// Auth:  K. Loux
// Copy:  (c) Copyright 2015
// Desc:  C++ wrapper for Wiring Pi interrupt functions.

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

// Local headers
#include "gpio.h"

class Interrupt : public GPIO
{
public:
	enum EdgeDirection
	{
		EdgeRising,
		EdgeFalling,
		EdgeBoth,
		EdgePreconfigured
	};

	typedef void InterruptServiceRoutine();

	Interrupt(const int &pin, const InterruptServiceRoutine isr,
		const EdgeDirection &edge = EdgeRising);
	virtual ~Interrupt() {}

private:
	const InterruptServiceRoutine isr;
};


#endif// INTERRUPT_H_
