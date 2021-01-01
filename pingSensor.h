// File:  pingSensor.h
// Date:  12/16/2020
// Auth:  K. Loux
// Copy:  (c) Copyright 2020
// Desc:  Ping sensor object.  Measures distance using ultrasonic sensor.

#ifndef PING_SENSOR_H_
#define PING_SENSOR_H_

// Local heades
#include "rpi/gpio.h"

// Standard C++ headers
#include <chrono>

class PingSensor
{
public:
	PingSensor(const unsigned int& triggerPin, const unsigned int& echoPin);
	
	bool GetDistance(double& distance);// [cm]

private:
	GPIO trigger;
	GPIO echo;
	
	void SendTrigger();
	
	typedef std::chrono::high_resolution_clock Clock;
	bool MeasureEchoPulse(Clock::duration& duration);
};

#endif// PING_SENSOR_H_
