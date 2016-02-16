// File:  twi.h
// Date:  2/15/2016
// Auth:  K. Loux
// Desc:  Object for handling TWI communication with a slave device.

#ifndef TWI_H_
#define TWI_H_

// Standard C++ headers
#include <string>
#include <vector>
#include <iostream>

class TWI
{
public:
	TWI(const std::string& deviceFileName, const unsigned char& address,
		std::ostream& outStream = std::cout);
	virtual ~TWI() {}

	bool Write(const std::vector<unsigned char>& data) const;
	bool Read(std::vector<unsigned char>& data) const;

	bool ConnectionOK() const;

	std::string GetErrorString() const;

private:
	const unsigned char address;
	std::ostream& outStream;

	int busFileDescriptor;
	static const unsigned int bufferSize;
	unsigned char buffer[bufferSize];
};

#endif// TWI_H_
