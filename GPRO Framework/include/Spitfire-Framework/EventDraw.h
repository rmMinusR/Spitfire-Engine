#pragma once
#include "Spitfire-Framework/BaseEvent.h"

class EventDraw : BaseEvent
{
	EventType::e x() override {
		return EventType::DRAW;
	}
};

