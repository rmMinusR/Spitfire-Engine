#include "Spitfire-Framework/textstyle.h"

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

TextStyle::TextStyle(bool r, bool g, bool b, bool light)
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

#pragma endregion

#pragma region StyledString

std::ostream& operator<<(std::ostream& out, const StyledString& s)
{
	for (int i = 0; i < s.length(); i++) {
		out << s.at(i);
	}
	return out;
}

void StyledString::setStyle(int start, int end, TextStyle style)
{
	for (int i = max(0, start); i < min(end, length()); i++) {
		at(i).style = style;
	}
}

#pragma endregion

