#include "Spitfire-Framework/textstyle.h"
#include "Spitfire-Framework/sfio.h"

#include <Windows.h> //For console text coloration, at the cost of portability

#pragma region Coloration utils

unsigned char ascol(bool r, bool g, bool b, bool light) {
	return 	r * 0b0100 +
		g * 0b0010 +
		b * 0b0001 +
		light * 0b1000;
}

void csetcolb(unsigned char col) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, col);
}

void csetcolc(bool r, bool g, bool b, bool light) {
	csetcolb(ascol(r, g, b, light));
}

#pragma endregion

#pragma region TextStyle

void TextStyle::regenStylecode()
{
	stylecode = ascol(r, g, b, light);
}

TextStyle::TextStyle() : TextStyle(1, 1, 1, 1) {}

TextStyle::TextStyle(const bool& r, const bool& g, const bool& b, const bool& light)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->light = light;

	stylecode = 0; //To suppress compiler error

	regenStylecode();
}

void TextStyle::setR    (const bool& r)     { this->r     = r;     regenStylecode(); }
void TextStyle::setG    (const bool& g)     { this->g     = g;     regenStylecode(); }
void TextStyle::setB    (const bool& b)     { this->b     = b;     regenStylecode(); }
void TextStyle::setLight(const bool& light) { this->light = light; regenStylecode(); }

void TextStyle::applyStyle() const
{
	csetcolb(stylecode);
}

std::ostream& operator<<(std::ostream& out, const TextStyle& style)
{
	style.applyStyle();
	return out;
}

#pragma endregion

#pragma region StyledChar

std::ostream& operator<<(std::ostream& out, const StyledChar& c)
{
	c.style.applyStyle();
	out << c.character;
	return out;
}

StyledChar::StyledChar()
{
	character = '\0';
	style = TextStyle();
}

StyledChar::StyledChar(const char& c)
{
	character = c;
	style = TextStyle();
}

StyledChar::StyledChar(const char& c, const TextStyle& s)
{
	character = c;
	style = s;
}

#pragma endregion

#pragma region StyledString

std::ostream& operator<<(std::ostream& out, const StyledString& s)
{
	for (int i = 0; i < s.length(); i++) {
		out << s.at(i);
	}
	return out;
}

void StyledString::setStyle(const TextStyle& style, const int& end = INT_MAX, const int& start = 0)
{
	for (int i = max(0, start); i < min(end, length()); i++) {
		at(i).style = style;
	}
}

#pragma endregion

#pragma region StyledTextBlock

StyledTextBlock::StyledTextBlock(const int& w, const int& h) {
	width = w;
	height = h;

	textBlock = new StyledChar * [w];
	for (int i = 0; i < w; i++) {
		textBlock[i] = new StyledChar[h];
		for (int j = 0; j < h; j++) textBlock[i][j] = StyledChar();
	}
}

StyledTextBlock::~StyledTextBlock() {
	for (int i = 0; i < width; i++) delete[] textBlock[i];
	delete[] textBlock;
}

void StyledTextBlock::setStyledChar(const StyledChar& sc, const int& x, const int& y)
{
	if (x >= width || y >= height || x < 0 || y < 0) {
		return;
	}

	setStyle(sc.style, x, y);
	setChar(sc.character, x, y);
}

void StyledTextBlock::setStyle(const TextStyle& style, const int& x, const int& y)
{
	if (x >= width || y >= height || x < 0 || y < 0) {
		return;
	}

	textBlock[x][y].style = style;
}

void StyledTextBlock::setChar(const char& chars, const int& x, const int& y)
{
	if (x >= width || y >= height || x < 0 || y < 0) {
		return;
	}

	textBlock[x][y].character = chars;
}

void StyledTextBlock::fillStyledChar(const StyledChar &sc, const int& x1, const int& y1, const int& x2, const int& y2)
{
	for (int ix = x1; ix <= x2; ix++) {
		for (int iy = y1; iy <= y2; iy++) {
			setStyledChar(sc, ix, iy);
		}
	}
}

void StyledTextBlock::fillStyle(const TextStyle& style, const int& x1, const int& y1, const int& x2, const int& y2)
{
	for (int ix = x1; ix <= x2; ix++) {
		for (int iy = y1; iy <= y2; iy++) {
			setStyle(style, ix, iy);
		}
	}
}

void StyledTextBlock::fillChar(const char& chars, const int& x1, const int& y1, const int& x2, const int& y2)
{
	for (int ix = x1; ix <= x2; ix++) {
		for (int iy = y1; iy <= y2; iy++) {
			setChar(chars, ix, iy);
		}
	}
}

void StyledTextBlock::putStr(const std::string& str, const int& x, const int& y)
{
	for (int i = 0; i < str.length() && i + x < width; i++) {
		setChar(str.at(i), i + x, y);
	}
}

void StyledTextBlock::putSstr(const StyledString& sstr, const int& x, const int& y)
{
	for (int i = 0; i < sstr.length() && i + x < width; i++) {
		setStyledChar(sstr.at(i), i + x, y);
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

void StyledTextBlock::drawBox(const TextStyle& style, int x1, int y1, int x2, int y2)
{
	setStyledChar(StyledChar(border_nw, style), x1, y1);
	setStyledChar(StyledChar(border_sw, style), x1, y2);
	setStyledChar(StyledChar(border_se, style), x2, y2);
	setStyledChar(StyledChar(border_ne, style), x2, y1);

	fillStyledChar(StyledChar(border_s, style), x1 + 1, y2,     x2 - 1, y2    );
	fillStyledChar(StyledChar(border_n, style), x1 + 1, y1,     x2 - 1, y1    );
	fillStyledChar(StyledChar(border_w, style), x1,     y1 + 1, x1,     y2 - 1);
	fillStyledChar(StyledChar(border_e, style), x2,     y1 + 1, x2,     y2 - 1);
}

void StyledTextBlock::renderAt(const int& x, const int& y)
{
	for (int iy = 0; iy < height; iy++) {
		csetcurpos(x, y + iy);
		for (int ix = 0; ix < width; ix++) {
			if (textBlock[ix][iy].character > 32 || textBlock[ix][iy].character < 0) std::cout << textBlock[ix][iy];
			else std::cout << " ";
		}
	}
}

#pragma endregion
