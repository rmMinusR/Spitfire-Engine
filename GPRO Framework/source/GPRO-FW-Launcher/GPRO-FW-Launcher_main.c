// GPRO-FW-Launcher.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>

#include "Spitfire-Framework/World.h"

int main()
{
	World w;

	w.OnTick();

	w.~World(); //Would be called anyway but I like being verbose
	return 0;
}
