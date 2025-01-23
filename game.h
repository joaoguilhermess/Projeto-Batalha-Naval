#ifndef GAME_INCLUDED
	#define GAME_INCLUDED

	#include "console.h"

	struct Bomb {
		int y;		
		int x;
		bool isSunked;		
	};

	struct Ship {
		bool isVisible;
		int y;
		int x;
		int size;
		bool isVertical;
		bool isSunked;
		bool isSelected;
	};

	struct Player {
		std::string name;
		std::array<Ship, SHIPS> ships;
		std::vector<Bomb> bombs;
	};

	struct Cell {
		bool isWater;
		bool hasBomb;
		bool hasShip;
		bool isSelected;
		bool isCollided;
		int shipSize;
		int shipIndex;
		bool isVertical;
	};

	void game();

	void setTitle(std::string title);

	void showWinner(Player& player1, Player& player2, std::array<std::array<Cell, GRID>, GRID>& grid, int row, int offset, Player& winner);

	void playTurn(Player& current, Player& other, std::array<std::array<Cell, GRID>, GRID>& grid, int row, int offset, bool& running, int shipCells, Player& winner);

	void moveShips(Player& current, std::array<std::array<Cell, GRID>, GRID>& grid, int row, int offset, bool& running);

	std::array<Ship, SHIPS> createShips();

	void clearGrid(std::array<std::array<Cell, GRID>, GRID>& grid);
	
	void setBombs(std::vector<Bomb>& bombs, std::array<std::array<Cell, GRID>, GRID>& grid, bool& collided);

	void setShips(std::array<Ship, SHIPS>& ships, std::array<std::array<Cell, GRID>, GRID>& grid, int& collided);

	void drawGrid(std::array<std::array<Cell, GRID>, GRID>& grid, int row, int offset);
#endif