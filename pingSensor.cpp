// File:  pingSensor.cpp
// Date:  12/16/2020
// Auth:  K. Loux
// Copy:  (c) Copyright 2020
// Desc:  Ping sensor object.  Measures distance using ultrasonic sensor.

// Local headers
#include "pingSensor.h"

// Standard C++ headers
#include <thread>
#include <cassert>

PingSensor::State PingSensor::state(PingSensor::State::Idle);
PingSensor::Clock::time_point PingSensor::startTime;
PingSensor::Clock::time_point PingSensor::stopTime;

PingSensor::PingSensor(const unsigned int& triggerPin, const unsigned int& echoPin)
	: trigger(triggerPin, GPIO::DataDirection::Output), echo(echoPin, &PingSensor::PingISR, Interrupt::EdgeDirection::Both)
{
	trigger.SetOutput(false);
	echo.SetPullUpDown(GPIO::PullResistance::Off);
}

bool PingSensor::GetDistance(double& distance)
{
	SendTrigger();
	auto duration(MeasureEchoPulse());

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
	std::this_thread::sleep_for(std::chrono::microseconds(10));
	trigger.SetOutput(false);
}

PingSensor::Clock::duration PingSensor::MeasureEchoPulse()
{
	assert(state == State::Idle);
	assert(!echo.GetInput() && "Cannot call MeasureEchoPulse when echo pin is high");
	
	state = State::WaitingForRisingEdge;
	auto maxDuration(std::chrono::milliseconds(20));
	std::this_thread::sleep_for(maxDuration);
	
	if (state != State::Done)
	{
		state = State::Idle;
		return maxDuration * 10;// Error response
	}
	
	state = State::Idle;
	return stopTime - startTime;
}

void PingSensor::PingISR()
{
	auto now(Clock::now());
	if (state == State::WaitingForRisingEdge)
	{
		startTime = now;
		state = State::WaitingForFallingEdge;
	}
	else if (state == State::WaitingForFallingEdge)
	{
		stopTime = now;
		state = State::Done;
	}
	else
		state = State::Idle;
}
