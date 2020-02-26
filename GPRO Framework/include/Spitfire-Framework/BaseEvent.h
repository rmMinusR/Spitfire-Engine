#pragma once

namespace EventType { //When inheriting from BaseEventListener, must register here.
	enum e {
		BASE,
		START,
		TICK,
		DRAW,
		CLEANUP
	};
}

class BaseEvent
{
public:
	virtual EventType::e x() = 0; //Reflection-like data
};