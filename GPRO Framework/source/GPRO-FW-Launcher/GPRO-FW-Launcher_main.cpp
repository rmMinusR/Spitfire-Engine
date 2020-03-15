// GPRO-FW-Launcher.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Game/GPRO-FW-TicTacToe.h"
#include "Spitfire-Framework/sfio.h"

int main()
{
	getMenuName(false).renderAt(0, 0);
	cquerych();
	return launchTicTacToe();
}
