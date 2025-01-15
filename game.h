#ifndef GAME_INCLUDED
	#define GAME_INCLUDED

	#include "console.h"

	void game();

	void drawGrid(int row, int offset);

	void drawBoat(int row, int offset, int y, int x, int size, int vertical);
#endif