// File:  twi.cpp
// Date:  2/15/2016
// Auth:  K. Loux
// Desc:  Object for handling TWI communication with a slave device.

// Standard C/C++ headers
#include <cassert>
#include <stdio.h>
//#include <stdlib.h>
//#include <fcntl.h>
//#include <string.h>
#include <sys/ioctl.h>
//#include <sys/types.h>
//#include <sys/stat.h>

// Linux headers
#include <linux/i2c-dev.h>
#include <unistd.h>

// Local headers
#include "twi.h"

const unsigned int TWI::bufferSize(10);

TWI::TWI(const std::string& deviceFileName, const unsigned char& address,
	std::ostream& outStream), address(address), outStream(outStream)
{
	busFileDescriptor = open(deviceFileName.c_str(), O_RDWR);
}

bool TWI::Write(const std::vector<unsigned char>& data) const
{
	assert(ConnectionOK());
	assert(data.size() > 0);
	assert(data.size() < bufferSize);

	if (ioctl(fd, I2C_SLAVE, address) == -1)
	{
		outStream << "Failed to get bus access:  " << GetErrorString() << std::endl;
		return false;
	}

	unsigned int i;
	for (i = 0; i < data.size(); i++)
		buffer[i] = data[i];

	int writeSize = write(fd, buffer, data.size());
	if (writeSize == -1)
	{
		outStream << "Failed to write to slave:  " << GetErrorString() << std::endl;
		return false;
	}
	else if (writeSize != data.size())
	{
		outStream << "Wrong number of bytes written" << std::endl;
		return false;
	}

	return true;
}

bool TWI::Read(std::vector<unsigned char>& data) const
{
	assert(ConnectionOK());

	int readSize = read(fd, buffer, bufferSize);
	if (readSize == -1)
	{
		outStream << "Failed to read from slave:  " << GetErrorString() << std::endl;
		return false;
	}

	data.resize(readSize);
	unsigned int i;
	for (i = 0; i < readSize; i++)
		data[i] = buffer[i];

	return true;
}

bool TWI::ConnectionOK() const
{
	return busFileDescriptor != -1;
}

std::string TWI::GetErrorString() const
{
	std::ostringstream ss;
	ss << "(" << errno << ") " << strerror(errno);
	return ss.str();
}
