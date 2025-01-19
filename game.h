#ifndef GAME_INCLUDED
	#define GAME_INCLUDED

	#include "console.h"

	struct Ship {
		bool isVisible;
		int y;
		int x;
		int size;
		bool isVertical;
		bool isSunked;
	};

	struct Cell {
		bool isWater;
		bool hasBomb;
		bool hasShip;
		int shipIndex;
		bool isVertical;
	};

	void game();

	std::array<Ship, SHIPS> createShips();

	void clearGrid(std::array<std::array<Cell, GRID>, GRID>& grid);

	bool verifyCollision(std::array<Ship, SHIPS>& ships);

	void setShips(std::array<Ship, SHIPS>& ships, std::array<std::array<Cell, GRID>, GRID>& grid);

	void drawGrid(std::array<std::array<Cell, GRID>, GRID>& grid, int row, int offset);
#endif