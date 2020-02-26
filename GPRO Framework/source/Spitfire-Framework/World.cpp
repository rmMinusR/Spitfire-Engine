#include "Spitfire-Framework/World.h"

World::World()
{
	EventCleanup e = EventCleanup();
	for (BaseEventListener* o : gameObjects) o->OnRecieveEventCleanup(e);

	while (gameObjects.size() > 0) delete gameObjects[0];
}

World::~World()
{
}

void World::OnTick()
{
	{
		//Tick event
		EventTick e = EventTick();
		for (BaseEventListener* o : gameObjects) o->OnRecieveEventTick(e);
	}

	{
		//Draw event
		EventDraw e = EventDraw();
		for (BaseEventListener* o : gameObjects) o->OnRecieveEventDraw(e);
	}
}

bool World::AddGameObject(BaseEventListener* o) {
	if (std::count(gameObjects.begin(), gameObjects.end(), o) > 0) return false;

	gameObjects.push_back(o);

	return true;
}

bool World::RemoveGameObject(BaseEventListener* o) {
	if (std::count(gameObjects.begin(), gameObjects.end(), o) < 0) return false;

	for (int i = 0; i < gameObjects.size(); i++) {

		if (gameObjects[i] == o) {
			gameObjects.erase(gameObjects.begin()+i);
			delete o;
			return true;
		}

	}

	return false;
}