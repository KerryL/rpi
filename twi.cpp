// File:  twi.cpp
// Date:  2/15/2016
// Auth:  K. Loux
// Desc:  Object for handling TWI communication with a slave device.

// Standard C/C++ headers
#include <cassert>
#include <sstream>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>

// Linux headers
#include <linux/i2c-dev.h>
#include <unistd.h>

// Local headers
#include "twi.h"

const unsigned int TWI::bufferSize(10);

TWI::TWI(const std::string& deviceFileName, const unsigned char& address,
	std::ostream& outStream) : address(address), outStream(outStream)
{
	busFileDescriptor = open(deviceFileName.c_str(), O_RDWR);
	buffer = new unsigned char[bufferSize];
}

TWI::~TWI()
{
	delete [] buffer;
}

bool TWI::Write(const std::vector<unsigned char>& data) const
{
	assert(ConnectionOK());
	assert(data.size() > 0);
	assert(data.size() < bufferSize);

	if (ioctl(busFileDescriptor, I2C_SLAVE, address) == -1)
	{
		outStream << "Failed to get bus access:  " << GetErrorString() << std::endl;
		return false;
	}

	unsigned int i;
	for (i = 0; i < data.size(); i++)
		buffer[i] = data[i];

	int writeSize = write(busFileDescriptor, buffer, data.size());
	if (writeSize == -1)
	{
		outStream << "Failed to write to slave:  " << GetErrorString() << std::endl;
		return false;
	}
	else if (writeSize != (int)data.size())
	{
		outStream << "Wrong number of bytes written" << std::endl;
		return false;
	}

	return true;
}

bool TWI::Read(std::vector<unsigned char>& data,
	const unsigned short& size) const
{
	assert(ConnectionOK());
	assert(size <= bufferSize);

	int readSize = read(busFileDescriptor, buffer, size);
	if (readSize == -1)
	{
		outStream << "Failed to read from slave:  " << GetErrorString() << std::endl;
		return false;
	}

	data.resize(readSize);
	int i;
	for (i = 0; i < readSize; i++)
		data[i] = buffer[i];

	return true;
}

bool TWI::ConnectionOK() const
{
	return busFileDescriptor != -1 && buffer;
}

std::string TWI::GetErrorString() const
{
	std::ostringstream ss;
	ss << "(" << errno << ") " << strerror(errno);
	return ss.str();
}
