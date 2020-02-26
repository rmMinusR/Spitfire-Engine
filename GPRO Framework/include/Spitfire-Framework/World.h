#pragma once

#include <vector>

#include "Spitfire-Framework/BaseEventListener.h"

class World
{
private:
	std::vector<BaseEventListener*> gameObjects;

public:
	World();
	~World();

	void OnTick();

	bool AddGameObject(BaseEventListener* o);
	bool RemoveGameObject(BaseEventListener* o);
};

