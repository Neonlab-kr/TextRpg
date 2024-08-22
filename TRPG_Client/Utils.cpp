#include "Utils.h"

void cursorView(bool show)
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;
	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

bool rateCheck(const int rate)
{
	int temp;
	srand(time(NULL));
	temp = rand() % 100 + 1;
	if (rate >= temp)
	{
		return true;
	}
	return false;
}

std::string paddingCenter(const std::string& text)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	int consoleWidth = csbi.dwSize.X;
	int textLength = text.length();
	int padding = (consoleWidth - textLength) / 2;
	std::string paddedText(padding, ' ');
	paddedText += text;
	return paddedText;
}

void printCentered(const std::string& text)
{
	std::cout << paddingCenter(text);
}

void blinkText(const std::string& text, int blinkDurationMs)
{
	while (!_kbhit()) {
		std::cout << "\r" + text << std::flush;
		Sleep(blinkDurationMs);

		std::cout << + "\r" + std::string(text.size(), ' ') << std::flush;
		Sleep(blinkDurationMs);
	}
}

void setcolor(unsigned short text, unsigned short back)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text | (back << 4));
}
