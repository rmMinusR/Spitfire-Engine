#include "Spitfire-Framework/sfio.h"

#include <Windows.h> //For console manipulation, at the cost of portability
#include <conio.h>
#include <string>

char cquerych() {
	return _getch();
}

void csetcurpos(unsigned int x, unsigned int y) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(hConsole, coord);
}

void cfill(char c, int x1, int y1, int x2, int y2) {
	for (int x = x1; x <= x2; x++) {
		for (int y = y1; y <= y2; y++) {
			csetcurpos(x, y); std::cout << c;
		}
	}
}

const char border_n = 196;
const char border_w = 179;
const char border_s = 196;
const char border_e = 179;

const char border_nw = 218;
const char border_sw = 192;
const char border_se = 217;
const char border_ne = 191;

void cdrawbox(int x1, int y1, int x2, int y2) {
	csetcurpos(x1, y1); std::cout << border_nw;
	csetcurpos(x1, y2); std::cout << border_sw;
	csetcurpos(x2, y2); std::cout << border_se;
	csetcurpos(x2, y1); std::cout << border_ne;

	cfill(border_n, x1+1, y1  , x2-1, y1  );
	cfill(border_s, x1+1, y2  , x2-1, y2  );
	cfill(border_w, x1  , y1+1, x1  , y2-1);
	cfill(border_e, x2  , y1+1, x2  , y2-1);
}

void renderarr(char* arr, const int& width, const int& height, const int& off_x, const int& off_y)
{
	const int border = 3;
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			int cell_x = border + x * (1 + border * 2);
			int cell_y = border + y * (1 + border * 2);

			csetcurpos(cell_x+off_x, cell_y+off_y);
			std::cout << *(arr+x+height*y);

			for(int i = 1; i <= border; i++) cdrawbox(cell_x - i + off_x, cell_y - i + off_y, cell_x + i + off_x, cell_y + i + off_y);
		}
	}

	csetcurpos(off_x, width * (1 + border * 2) + off_y);
}

void renderarr_styled(char* arr, unsigned char* styles, const int& width, const int& height, const int& off_x, const int& off_y)
{
	const int border = 3;
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			int cell_x = border + x * (1 + border * 2);
			int cell_y = border + y * (1 + border * 2);

			csetcurpos(cell_x + off_x, cell_y + off_y);
			csetcolb(*(styles + x + height * y));
			std::cout << *(arr + x + height * y);

			for (int i = 1; i <= border; i++) cdrawbox(cell_x - i + off_x, cell_y - i + off_y, cell_x + i + off_x, cell_y + i + off_y);
		}
	}

	csetcurpos(off_x, width * (1 + border * 2) + off_y);
	csetcolc(1, 1, 1, 1);
}

void renderarr_styled(char* arr, TextStyle* styles, const int& width, const int& height, const int& off_x, const int& off_y)
{
	const int border = 3;
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			int cell_x = border + x * (1 + border * 2);
			int cell_y = border + y * (1 + border * 2);

			csetcurpos(cell_x + off_x, cell_y + off_y);
			(styles + x + height * y)->applyStyle();
			std::cout << *(arr + x + height * y);

			for (int i = 1; i <= border; i++) cdrawbox(cell_x - i + off_x, cell_y - i + off_y, cell_x + i + off_x, cell_y + i + off_y);
		}
	}

	csetcurpos(off_x, width * (1 + border * 2) + off_y);
	csetcolc(1, 1, 1, 1);
}

void cclear()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD cwritten;
	DWORD csize;
	CONSOLE_SCREEN_BUFFER_INFO cinfo;
	COORD ccoords = { 0, 0 };

	GetConsoleScreenBufferInfo(hConsole, &cinfo) &&
	(csize = cinfo.dwSize.X * cinfo.dwSize.Y) &&
	FillConsoleOutputCharacter(hConsole, (TCHAR)' ', csize, ccoords, &cwritten) &&
	GetConsoleScreenBufferInfo(hConsole, &cinfo) &&
	FillConsoleOutputAttribute(hConsole, cinfo.wAttributes, csize, ccoords, &cwritten) &&
	SetConsoleCursorPosition(hConsole, { 0,0 });
}
