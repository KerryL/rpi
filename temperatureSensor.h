// File:  temperatureSensor.h
// Date:  8/31/2013
// Auth:  K. Loux
// Copy:  (c) Copyright 2013
// Desc:  Temperature sensor object.  For use with a DS18B20 "1-wire"
//        temperature sensor.  This is implemented using system calls, as the
//        1-wire interface is built into the Raspian kernel.  Note that
//        for this to work, the sensor must be connected to GPIO7.

#ifndef TEMPERATURE_SENSOR_H_
#define TEMPERATURE_SENSOR_H_

class TemperatureSensor
{
public:
	virtual ~TemperatureSensor() {}

	virtual bool GetTemperature(double &temperature) const = 0;// [deg C]

private:
};

#endif// TEMPERATURE_SENSOR_H_
