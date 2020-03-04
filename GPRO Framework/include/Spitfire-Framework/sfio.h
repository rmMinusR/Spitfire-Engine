#pragma once

#include "Spitfire-Framework/textstyle.h"

#include <iostream>
#include <string>

char cquerych();

void csetcurpos(unsigned int x, unsigned int y);

void renderarr       (char* arr,                        const int& width, const int& height, const int& off_x = 0, const int& off_y = 0);
void renderarr_styled(char* arr, unsigned char* styles, const int& width, const int& height, const int& off_x = 0, const int& off_y = 0);
void renderarr_styled(char* arr,     TextStyle* styles, const int& width, const int& height, const int& off_x = 0, const int& off_y = 0);

void cclear();