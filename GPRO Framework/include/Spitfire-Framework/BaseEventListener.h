#pragma once

#include <string>

#include "Spitfire-Framework/BaseEvent.h"
#include "Spitfire-Framework/EventStart.h"
#include "Spitfire-Framework/EventDraw.h"
#include "Spitfire-Framework/EventCleanup.h"
#include "Spitfire-Framework/EventTick.h"

class BaseEventListener
{
private:
	int serialID;
	static int nextSerialID;

protected:
	BaseEventListener();

public:
	virtual std::string ToString();

	virtual bool OnRecieveEventStart  (EventStart  & recievedEvent);
	virtual bool OnRecieveEventTick   (EventTick   & recievedEvent);
	virtual bool OnRecieveEventDraw   (EventDraw   & recievedEvent);
	virtual bool OnRecieveEventCleanup(EventCleanup& recievedEvent);
};