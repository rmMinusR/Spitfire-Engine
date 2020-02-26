#pragma once

#include "Spitfire-Framework/BaseEvent.h"

class EventStart : BaseEvent
{
	EventType::e x() override {
		return EventType::START;
	}
};

