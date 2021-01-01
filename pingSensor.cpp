// File:  pingSensor.cpp
// Date:  12/16/2020
// Auth:  K. Loux
// Copy:  (c) Copyright 2020
// Desc:  Ping sensor object.  Measures distance using ultrasonic sensor.

// Local headers
#include "pingSensor.h"

// WiringPi headers
#include <wiringPi.h>

// Standard C++ headers
#include <thread>
#include <cassert>

PingSensor::PingSensor(const unsigned int& triggerPin, const unsigned int& echoPin)
	: trigger(triggerPin, GPIO::DataDirection::Output), echo(echoPin, GPIO::DataDirection::Input)
{
	trigger.SetOutput(false);
	echo.SetPullUpDown(GPIO::PullResistance::Off);
}

bool PingSensor::GetDistance(double& distance)
{
	SendTrigger();
	Clock::duration duration;
	if (!MeasureEchoPulse(duration))
		return false;

	const long long speedOfSound(34300);// [cm/sec]
	const auto maxDuration(std::chrono::nanoseconds(1000000000LL * 500LL / speedOfSound));
	if (duration > maxDuration * 1.05)// Allow some fudge room
		return false;
	
	distance = 0.5 * std::chrono::duration<double>(duration).count() * speedOfSound;
	return true;
}

void PingSensor::SendTrigger()
{
	trigger.SetOutput(true);
	delayMicroseconds(10);
	trigger.SetOutput(false);
}

bool PingSensor::MeasureEchoPulse(Clock::duration& duration)
{
	const unsigned int timeoutDuration(100);// [ms]
	const auto maxTime(Clock::now() + std::chrono::milliseconds(timeoutDuration));
	Clock::time_point startTime(Clock::now());
	Clock::time_point stopTime(Clock::now());
	
	// Wait for rising edge
	while (!echo.GetInput() && startTime < maxTime)
		startTime = Clock::now();
		
	// Wait for falling edge
	while (echo.GetInput() && stopTime < maxTime)
		stopTime = Clock::now();
		
	if (stopTime < startTime || startTime >= maxTime || stopTime >= maxTime)
		return false;
		
	duration = stopTime - startTime;
	return true;
}
