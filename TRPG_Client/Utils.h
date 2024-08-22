#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <conio.h>

void cursorView(bool show);
bool rateCheck(const int rate);
std::string paddingCenter(const std::string& text);
void printCentered(const std::string& text);
void blinkText(const std::string& text, int blinkDurationMs);
void setcolor(unsigned short text, unsigned short back);