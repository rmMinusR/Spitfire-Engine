#pragma once

#include <string>

class BaseEventListener
{
private:
	int serialID;
	static int nextSerialID;

protected:
	BaseEventListener();

public:
	virtual std::string ToString();
};

