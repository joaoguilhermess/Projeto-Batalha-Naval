	#ifndef CONSOLE_INCLUDED
	#define CONSOLE_INCLUDED

	#include <windows.h>
	#include <iostream>
	#include <thread>
	#include <string>
	#include <vector>
	#include "values.h"
	#include "conio.h"

	void adjustConsole();
	
	void adjustFontSize();
	
	void resizeBuffer();

	void enableCursor(bool enable);
	void setCursor(int y, int x);
	
	void setFullscreen();
	
	void delay();
	
	int getRows();
	int getColumns();

	int getLength(std::string text);

	void clearConsole();
	
	void setColor(WORD color);

	void leftText(std::string text, int row, int offset);
	
	void centerText(std::string text, int row, int offset);
	
	void rightText(std::string text, int row, int offset);

	void centerBorder(int rows, int columns, int row, int offset);

	void drawButton(std::string text, int row, int offset, bool selected);

	void drawText(std::string title, int columns, std::vector<std::string> lines);

	std::string getInput();
#endif