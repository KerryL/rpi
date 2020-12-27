// File:  gpio.h
// Date:  8/30/2013
// Auth:  K. Loux
// Copy:  (c) Copyright 2013
// Desc:  C++ wrapper for Wiring Pi general-purpose I/O function calls.

#ifndef GPIO_H_
#define GPIO_H_

class GPIO
{
public:
	enum class DataDirection
	{
		Input,
		Output,
		PWMOutput
	};

	enum class PullResistance
	{
		Off,
		PullUp,
		PullDown
	};

	GPIO(const int &pin, const DataDirection &direction);
	virtual ~GPIO();

	void SetDataDirection(const DataDirection &direction);
	void SetPullUpDown(const PullResistance &state);
	void SetOutput(const bool &high);
	bool GetInput();

protected:
	const int pin;

private:
	DataDirection direction;
	static bool initialized;
};


#endif// GPIO_H_
