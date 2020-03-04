#pragma once
#include <ostream>

unsigned char ascol(bool r, bool g, bool b, bool light);
void csetcolb(unsigned char col);
void csetcolc(bool r, bool g, bool b, bool light);

class TextStyle {
	bool r;
	bool g;
	bool b;
	bool light;

	unsigned char stylecode;

	void regenStylecode();

public:
	TextStyle();
	TextStyle(bool r, bool g, bool b, bool light);

	void setR(const bool& r);
	void setG(const bool& g);
	void setB(const bool& b);
	void setLight(const bool& light);

	void applyStyle() const;

	friend std::ostream& operator<<(std::ostream& out, const TextStyle& style);
};

class StyledChar {
public:
	char character;
	TextStyle style;

	friend std::ostream& operator<<(std::ostream& out, const StyledChar& c);
};

//Modified definition of std::string from xstring
//using StyledString = std::basic_string<StyledChar, std::char_traits<StyledChar>, std::allocator<StyledChar>>;

class StyledString : public std::basic_string<StyledChar, std::char_traits<StyledChar>, std::allocator<StyledChar>> {
public:
	void setStyle(int start, int end, TextStyle style);

	friend std::ostream& operator<<(std::ostream& out, const StyledString& s);
};