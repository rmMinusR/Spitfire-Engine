#include <sstream>

#include "Spitfire-Framework/BaseEventListener.h"

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

bool BaseEventListener::OnRecieveEventStart(EventStart& recievedEvent)
{
	return false;
}

bool BaseEventListener::OnRecieveEventTick(EventTick& recievedEvent)
{
	return false;
}

bool BaseEventListener::OnRecieveEventDraw(EventDraw& recievedEvent)
{
	return false;
}

bool BaseEventListener::OnRecieveEventCleanup(EventCleanup& recievedEvent)
{
	return false;
}
