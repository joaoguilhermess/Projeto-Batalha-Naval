#ifndef MAIN_INCLUDED
	#define MAIN_INCLUDED

	#include "console.h"
	#include "rules.h"
	#include "game.h"
	#include "credits.h"

	void splash();
	
	void drawTitle(int& row);

	void drawButtons(int& row, int selected);

	void drawButton(std::string text, int length, int row, int offset, bool selected);
	void drawButton(std::string text, int row, int offset, bool selected);
#endif