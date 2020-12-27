// File:  pingSensor.h
// Date:  12/16/2020
// Auth:  K. Loux
// Copy:  (c) Copyright 2020
// Desc:  Ping sensor object.  Measures distance using ultrasonic sensor.

#ifndef PING_SENSOR_H_
#define PING_SENSOR_H_

// Local heades
#include "rpi/interrupt.h"

// Standard C++ headers
#include <chrono>

class PingSensor
{
public:
	PingSensor(const unsigned int& triggerPin, const unsigned int& echoPin);
	
	bool GetDistance(double& distance);// [cm]

private:
	GPIO trigger;
	Interrupt echo;
	
	void SendTrigger();
	
	typedef std::chrono::high_resolution_clock Clock;
	Clock::duration MeasureEchoPulse();
	
	static void PingISR();
	
	enum class State
	{
		Idle,
		WaitingForRisingEdge,
		WaitingForFallingEdge,
		Done
	};
	
	// TODO:  Note use of static methods here prevent use of more than one Ping))) sensor at a time.
	// Calling code must ensure we're only requesting measurements from one device at a time.
	static State state;

	static Clock::time_point startTime;
	static Clock::time_point stopTime;
};

#endif// PING_SENSOR_H_
