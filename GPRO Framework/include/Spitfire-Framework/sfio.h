#pragma once

#include "Spitfire-Framework/textstyle.h"

#include <iostream>
#include <string>

char cquerych();
char cquerycht(float timeout);

void csetcurpos(unsigned int x, unsigned int y);
int cgetw();
int cgeth();

void csetcurvis(bool visiblity);

void cfill(char c, int x1, int y1, int x2, int y2);
void cdrawbox(int x1, int y1, int x2, int y2);

void cclear();

void showDialog(std::string str, int x, int y);