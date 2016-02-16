// File:  twi.h
// Date:  2/15/2016
// Auth:  K. Loux
// Desc:  Object for handling TWI communication with a slave device.

#ifndef TWI_H_
#define TWI_H_

// Standard C++ headers
#include <string>
#include <vector>
#include <ostream>

class TWI
{
public:
	TWI(const unsigned char& address, std::ostream& outStream = std::cout);
	~TWI();

	bool Open(const std::string& deviceFileName);

	bool Write(const std::vector<unsigned char>& data) const;
	bool Read(std::vector<unsigned char>& data) const;

	bool ConnectionOK() const { return connectionOK; }

private:
	const unsigned char address;
	std::ostream& outStream;
	bool connectionOK;

	int fd;
};

#endif// TWI_H_
