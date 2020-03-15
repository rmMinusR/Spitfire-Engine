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
	TextStyle(const bool& r, const bool& g, const bool& b, const bool& light);

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

	StyledChar();
	StyledChar(const char& c);
	StyledChar(const char& c, const TextStyle& s);

	friend std::ostream& operator<<(std::ostream& out, const StyledChar& c);
};

class StyledString : public std::basic_string<StyledChar, std::char_traits<StyledChar>, std::allocator<StyledChar>> {
public:
	void setStyle(const TextStyle& style, const int& end, const int& start);

	friend std::ostream& operator<<(std::ostream& out, const StyledString& s);
};

class StyledTextBlock {
	StyledChar** textBlock;

public:
	int width, height;
	
	StyledTextBlock(const int& w, const int& h);
	StyledTextBlock(const std::string& src);
	StyledTextBlock(const StyledTextBlock& src);
	~StyledTextBlock();

	void setStyledChar(const StyledChar& sc,    const int& x, const int& y);
	void setStyle     (const  TextStyle& style, const int& x, const int& y);
	void setChar      (const       char& chars, const int& x, const int& y);

	void fillStyledChar(const StyledChar& styledChar, const int& x1, const int& y1, const int& x2, const int& y2);
	void fillStyle     (const  TextStyle& style,      const int& x1, const int& y1, const int& x2, const int& y2);
	void fillChar      (const       char& chars,      const int& x1, const int& y1, const int& x2, const int& y2);

	void putStr (const  std::string&  str, const int& x, const int& y);
	void putSstr(const StyledString& sstr, const int& x, const int& y);

	void drawBox(const TextStyle& style, int x1, int y1, int x2, int y2);

	void renderAt(const int& x, const int& y);
};
