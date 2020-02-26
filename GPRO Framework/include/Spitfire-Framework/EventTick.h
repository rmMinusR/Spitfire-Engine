#pragma once
#include "BaseEvent.h"
class EventTick : BaseEvent
{
	EventType::e x() override {
		return EventType::TICK;
	}

public:
	double timeSinceLastTick;
};
