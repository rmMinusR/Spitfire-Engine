#include "Spitfire-Framework/BaseEventListener.h"

#include <sstream>

BaseEventListener::BaseEventListener()
{
	serialID = nextSerialID++;
}

std::string BaseEventListener::ToString()
{
	std::stringstream sout;

	sout << "{";

	sout << "BaseEventListener@" << this;
	sout << ", id:" << serialID;

	sout << "}";

	return sout.str();
}
