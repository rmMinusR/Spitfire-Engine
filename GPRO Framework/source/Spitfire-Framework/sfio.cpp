#include "Spitfire-Framework/sfio.h"

#include <Windows.h> //For console text coloration, at the cost of portability
#include <conio.h>
#include <string>

char cquerych() {
	return _getch();
}

unsigned char ascol(bool r, bool g, bool b, bool light) {
	return 	r*0b0001 + 
			g*0b0010 +
			b*0b0100 +
		light*0b1000;
}


void csetcolb(unsigned char col) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, col);
	CloseHandle(hConsole);
}

void csetcolc(bool r, bool g, bool b, bool light) {
	csetcolb(ascol(r, g, b, light));
}

void csetcurpos(unsigned int x, unsigned int y) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(hConsole, coord);
	CloseHandle(hConsole);
}