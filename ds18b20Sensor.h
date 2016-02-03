// File:  ds18b20Sensor.h
// Date:  8/31/2013
// Auth:  K. Loux
// Copy:  (c) Copyright 2013
// Desc:  Temperature sensor object.  For use with a DS18B20 "1-wire"
//        temperature sensor.  This is implemented using system calls, as the
//        1-wire interface is built into the Raspian kernel.  Note that
//        for this to work, the sensor must be connected to GPIO7.

#ifndef DS18B20_SENSOR_H_
#define DS18B20_SENSOR_H_

// Standard C++ headers
#include <string>
#include <vector>
#include <ostream>
#include <iostream>

// Local headers
#include "temperatureSensor.h"

class DS18B20 : public TemperatureSensor
{
public:
	DS18B20(std::string deviceID, std::ostream& outStream = std::cout,
		std::string baseDirectory = "/sys/bus/w1/devices/", const unsigned int& allowedRecursions = 3);

	virtual bool GetTemperature(double &temperature) const;// [deg C]

	static std::vector<std::string> GetConnectedSensors(
		std::string searchDirectory = "/sys/bus/w1/devices/");
	static bool DeviceIsDS18B20(std::string rom);

private:
	static bool initialized;
	static const std::string deviceFile;

	const std::string deviceID, device;
	std::ostream &outStream;
	const unsigned int allowedRecursions;

	bool ReadSensor(double &temperature, unsigned int recursion) const;// [deg C]
};

#endif// DS18B20_SENSOR_H_
