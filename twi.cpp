// File:  twi.cpp
// Date:  2/15/2016
// Auth:  K. Loux
// Desc:  Object for handling TWI communication with a slave device.

// Standard C headers
#include <stdio.h>
#include <stdlib.h>
//#include <fcntl.h>
//#include <string.h>
#include <sys/ioctl.h>
//#include <sys/types.h>
//#include <sys/stat.h>

// Linux headers
#include <linux/i2c-dev.h>
//#include <unistd.h>

// Local headers
#include "twi.h"

TWI::TWI(const std::string& address, std::ostream& outStream),
	address(address), outStream(outStream)
{
	fd = -1;
	connectionOK = false;
}

TWI::~TWI()
{
	if (fd >= 0)
		close(fd);
}

bool TWI::Open(const std::string& deviceFileName)
{
	fd = open(deviceFileName.c_str(), O_RDWR);
	if (fd < 0)
	{
		outStream << "Failed to open twi port" << std::endl;
		connectionOK = false;
	}
	else
		connectionOK = true;

	return connectionOK;
}

bool TWI::Write(const std::vector<unsigned char>& data) const
{
	if (ioctl(fd, I2C_SLAVE, address) < 0)
	{
		outStream << "Failed to get bus access" << std::endl;
		connectionOK = false;
		return false;
	}

	unsigned int i;
	for (i = 0; i < data.size(); i++)
	{
		if (write(fd, data[i], 1) != 1)
		{
			outStream << "Failed to write to slave" << std::endl;
			connectionOK = false;
			return false;
		}
	}

	return true;
}

bool TWI::Read(std::vector<unsigned char>& data) const
{
	const unsigned int bufferSize(10);
	unsigned char buffer[bufferSize];
	int readSize = read(fd, buffer, bufferSize)

	if (readSize <= 0)
	{
		outStream << "Failed to read from slave" << std::endl;
		connectionOK = false;
		return false;
	}

	data.resize(readSize);
	unsigned int i;
	for (i = 0; i < readSize; i++)
		data[i] = buffer[i];

	return true;
}

