// File:  temperatureSensor.cpp
// Date:  9/17/2013
// Auth:  K. Loux
// Copy:  (c) Copyright 2013
// Desc:  Temperature sensor object.  For use with a DS18B20 "1-wire"
//        temperature sensor.  This is implemented using system calls, as the
//        1-wire interface is built into the Raspian kernel.  Note that
//        for this to work, the sensor must be connected to GPIO7.

// Standard C++ headers
#include <cstdlib>
#include <fstream>
#include <cassert>

// *nix standard headers
#include <dirent.h>

// Local headers
#include "temperatureSensor.h"

//==========================================================================
// Class:			TemperatureSensor
// Function:		None
//
// Description:		Static and constant member initialization/definitions for
//					TemperatureSensor class.
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
bool TemperatureSensor::initialized = false;
const std::string TemperatureSensor::deviceFile = "/w1_slave";

//==========================================================================
// Class:			TemperatureSensor
// Function:		TemperatureSensor
//
// Description:		Constructor for TemperatureSensor class.
//
// Input Arguments:
//		deviceID		= std::string, unique idenfier for sensor
//		outStream		= std::ostream& (optional)
//		baseDirectory	= std::string (optional, default should be fine for Raspian OS)
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
TemperatureSensor::TemperatureSensor(std::string deviceID,
	std::ostream &outStream, std::string baseDirectory)
	: deviceID(deviceID), device(baseDirectory + deviceID + deviceFile), outStream(outStream)
{
	if (!initialized)
	{
		system("modprobe w1-gpio");
		system("modprobe w1-therm");
		initialized = true;
	}
}

//==========================================================================
// Class:			TemperatureSensor
// Function:		GetTemperature
//
// Description:		Reads current temperature from DS18B20 sensor.
//
// Input Arguments:
//		allowedRecursion	= unsigned int, number of attempts to make in the event of repeated errors
//
// Output Arguments:
//		temperature	= double& [deg C]
//
// Return Value:
//		bool, true for success, false otherwise
//
//==========================================================================
bool TemperatureSensor::GetTemperature(double &temperature, unsigned int allowedRecursions) const
{
	if (allowedRecursions == 0)
		return false;
		
	std::ifstream file(device.c_str(), std::ios::in);
	if (!file.is_open() || !file.good())
	{
		outStream << "Could not open file '" << device << "' for input" << std::endl;
		return GetTemperature(temperature, allowedRecursions - 1);
	}

	std::string data;
	if (!std::getline(file, data))
	{
		outStream << "Failed to read CRC from file '" << device << "'" << std::endl;
		return GetTemperature(temperature, allowedRecursions - 1);
	}

	// Line must contain at least "YES" at the end...
	if (data.length() < 3)
	{
		outStream << "File contents too short (" << deviceID << ")" << std::endl;
		return GetTemperature(temperature, allowedRecursions - 1);
	}

	if (data.substr(data.length() - 3).compare("YES") != 0)
	{
		// This happens quite often - might want to disable this statement
		// after testing is complete, to avoid spamming?
		outStream << "Bad checksum (" << deviceID << ")" << std::endl;
		return GetTemperature(temperature, allowedRecursions - 1);
	}

	if (!std::getline(file, data))
	{
		outStream << "Failed to read temperature from file '" << device << "'" << std::endl;
		return GetTemperature(temperature, allowedRecursions - 1);
	}

	size_t start(data.find("t="));
	if (start == std::string::npos)
	{
		outStream << "Temperature reading does not contain 't='"
			<< " (" << deviceID << ")" << std::endl;
		return GetTemperature(temperature, allowedRecursions - 1);
	}

	temperature = atof(data.substr(start + 2).c_str()) / 1000.0;

	return true;
}

//==========================================================================
// Class:			TemperatureSensor
// Function:		GetConnectedSensors
//
// Description:		Returns vector of connected DS18B20 sensors.
//
// Input Arguments:
//		searchDirectory	= std::string (optional)
//
// Output Arguments:
//		None
//
// Return Value:
//		std::vector<std::string>
//
//==========================================================================
std::vector<std::string> TemperatureSensor::GetConnectedSensors(std::string searchDirectory)
{
	// Assume that this might be called when a new sensor is connected - therefore, don't
	// use the initialized variable to determine whether or not to make these calls
	system("modprobe w1-gpio");
	system("modprobe w1-therm");

	std::vector<std::string> deviceList;

	// Initially, populate deviceList with all directories under searchDirectory
	DIR *directory = opendir(searchDirectory.c_str());
	if (!directory)
	{
		std::cout << "Failed to open directory file for '" << searchDirectory << "'" << std::endl;
		return deviceList;
	}

	struct dirent* listing;
	std::string name;
	while (listing = readdir(directory), listing != NULL)
	{
		// We could add a check here to make sure the listing is a directory,
		// but it's probably not necessary
		name = listing->d_name;
		if (name.length() == 15 &&
			name[2] == '-')
			deviceList.push_back(name);
	}

	if (closedir(directory) == -1)
		std::cout << "Failed to close directory file" << std::endl;

	unsigned int i;
	for (i = 0; i < deviceList.size(); i++)
	{
		if (!TemperatureSensor::DeviceIsDS18B20(deviceList[i]))
		{
			deviceList.erase(deviceList.begin() + i);
			i--;
		}
	}

	return deviceList;
}

//==========================================================================
// Class:			TemperatureSensor
// Function:		DeviceIsDS18B20
//
// Description:		Checks to make sure specified ROM is that of a DS18B20.
//
// Input Arguments:
//		rom	= std::string
//
// Output Arguments:
//		None
//
// Return Value:
//		bool, true for yes, false otherwise
//
//==========================================================================
bool TemperatureSensor::DeviceIsDS18B20(std::string rom)
{
	assert(rom.length() > 2);

	return rom.substr(0, 2).compare("28") == 0;
}
