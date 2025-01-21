#ifndef GAME_INCLUDED
	#define GAME_INCLUDED

	#include "console.h"

	struct Bomb {
		int y;		
		int x;
		bool isSunked;		
	}

	struct Ship {
		bool isVisible;
		int y;
		int x;
		int size;
		bool isVertical;
		bool isSunked;
	};

	struct Player {
		std::string name;
		std::array<Ship, SHIPS> ships;
		std::vector<Bomb> bombs;
	}

	struct Cell {
		bool isWater;
		bool hasBomb;
		bool hasShip;
		bool isSelected;
		bool isCollided;
		int shipIndex;
		bool isVertical;
	};

	void game();

	std::array<Ship, SHIPS> createShips();

	void moveShips(std::array<Ship, SHIPS>& ships, std::array<std::array<Cell, GRID>, GRID>& grid, int row, int offset, bool& running);

	void clearGrid(std::array<std::array<Cell, GRID>, GRID>& grid);

	void setShips(std::array<Ship, SHIPS>& ships, std::array<std::array<Cell, GRID>, GRID>& grid, bool& collided);

	void drawGrid(std::array<std::array<Cell, GRID>, GRID>& grid, int row, int offset);
#endif