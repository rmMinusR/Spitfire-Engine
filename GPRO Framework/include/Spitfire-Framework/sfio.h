#pragma once

#include <iostream>
#include <string>

char cquerych();

unsigned char ascol(bool r, bool g, bool b, bool light);
void csetcolb(unsigned char col);
void csetcolc(bool r, bool g, bool b, bool light);

void csetcurpos(unsigned int x, unsigned int y);

void renderarr       (char* arr,                        const int& width, const int& height, const int& off_x = 0, const int& off_y = 0);
void renderarr_styled(char* arr, unsigned char* styles, const int& width, const int& height, const int& off_x = 0, const int& off_y = 0);