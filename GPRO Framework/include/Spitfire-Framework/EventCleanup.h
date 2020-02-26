#pragma once

#include "Spitfire-Framework/BaseEvent.h"

class EventCleanup : BaseEvent
{
	EventType::e x() override {
		return EventType::CLEANUP;
	}
};

