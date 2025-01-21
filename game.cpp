#include "game.h"

void game() {
	clearConsole();

	std::array<Ship, SHIPS> ships = createShips();

	std::array<std::array<Cell, GRID>, GRID> leftGrid;
	std::array<std::array<Cell, GRID>, GRID> rightGrid;

	int selectedShip = 0;

	bool isRotating = true;
	bool isMovingShip = true;

	bool collided = false;

	while (true) {
		std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();

		adjustConsole();

		Ship& ship = ships[selectedShip];

		ship.isVisible = true;

		while (true) {
			std::string input = getInput();

			if (input == "") {
				break;
			}

			if (input == "exit") {
				return;
			} else if (isRotating) {
				if (input == "left") {
					ship.isVertical = true;
				} else if (input == "right") {
					ship.isVertical = false;
				} else if (input == "enter") {
					isRotating = false;
				}
			} else if (isMovingShip) {
				if (input == "top") {
					if (ship.y > 0) {
						ship.y -= 1;
					}
				} else if (input == "bottom") {
					if (ship.isVertical) {
						if (ship.y < GRID - ship.size) {
							ship.y += 1;
						}
					} else {
						if (ship.y < GRID - 1) {
							ship.y += 1;
						}
					}
				} else if (input == "left") {
					if (ship.x > 0) {
						ship.x -= 1;
					}
				} else if (input == "right") {
					if (ship.isVertical) {
						if (ship.x < GRID - 1) {
							ship.x += 1;
						}
					} else {
						if (ship.x < GRID - ship.size) {
							ship.x += 1;
						}
					}
				} else if (input == "enter") {
					if (!collided) {
						if (selectedShip < SHIPS) {
							selectedShip += 1;

							isRotating = true;
							isMovingShip = true;
						} else {
							isMovingShip = false;
						}
					}
				}
			}
		}

		clearGrid(leftGrid);
		clearGrid(rightGrid);

		setShips(ships, leftGrid, collided);

		int offset = getColumns() / 4;

		int row = (ROWS / 2);

		drawGrid(leftGrid, row, -offset);
		drawGrid(rightGrid, row, offset);

		std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();

		std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

		setColor(0);

		delay(duration.count());
	}
}

std::array<Ship, SHIPS> createShips() {
	std::array<Ship, SHIPS> ships;

	for (int i = 0; i < SHIPS; i++) {
		Ship&ship = ships[i];

		ship.isVisible = false;

		ship.y = 0;
		ship.x = 0;

		ship.size = SHIPS - i + 1;

		ship.isVertical = false;
		ship.isSunked = false;
	}

	return ships;
}

void clearGrid(std::array<std::array<Cell, GRID>, GRID>& grid) {
	for (int r = 0; r < GRID; r++) {
		for (int c = 0; c < GRID; c++) {
			Cell& cell = grid[r][c];

			cell.isWater = true;
			cell.hasShip = false;
			cell.hasBomb = false;
			cell.isSelected = false;
			cell.isCollided = false;
			cell.shipIndex = 0;
			cell.isVertical = false;
		}
	}
}

void setShips(std::array<Ship, SHIPS>& ships, std::array<std::array<Cell, GRID>, GRID>& grid, bool& collided) {
	collided = false;

	for (int s = 0; s < SHIPS; s++) {
		Ship& ship = ships[s];

		if (ship.isVisible) {
			for (int i = 0; i < ship.size; i++) {
				Cell* cell;

				if (ship.isVertical) {
					cell = &grid[ship.y + i][ship.x];
				} else {
					cell = &grid[ship.y][ship.x + i];
				}

				if (cell->hasShip) {
					collided = true;

					cell->isCollided = true;
				}

				cell->isWater = false;
				cell->hasShip = true;
				cell->shipIndex = i;
				cell->isVertical = ship.isVertical;
			}
		}
	}
}

void drawGrid(std::array<std::array<Cell, GRID>, GRID>& grid, int row, int offset) {
	row -= GRID;

	offset -= (GRID - 1) * 2;

	for (int r = 0; r < GRID; r++) {
		for (int c = 0; c < GRID; c++) {
			Cell& cell = grid[r][c];

			if (cell.isWater) {
				setColor(BACKGROUND_BLUE | BACKGROUND_INTENSITY);

				centerText("    ", row + (r * 2), offset + (c * 4));
				centerText("    ", row + (r * 2) + 1, offset + (c * 4));
			} else if (cell.hasShip) {
				if (cell.isCollided) {
					setColor(BACKGROUND_BLUE | BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_INTENSITY);
				} else {
					setColor(BACKGROUND_BLUE | BACKGROUND_INTENSITY | FOREGROUND_INTENSITY);
				}

				centerText("████", row + (r * 2), offset + (c * 4));
				centerText("████", row + (r * 2) + 1, offset + (c * 4));
			}
		}
	}
}

// void drawBoat(int row, int offset, int y, int x, int size, int vertical) {
// 	row += -GRID + (y - 1);

// 	offset += -(GRID * 2) + 2 + ((x - 1) * 4);

// 	setColor(BACKGROUND_BLUE | BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	
// 	for (int i = 0; i < size; i++) {
// 		if (i == 0) {
// 			if (vertical) {
// 				centerText("▄██▄", (row * 2) + (i * 2), offset);	
// 				centerText("████", (row * 2) + (i * 2) + 1, offset);
// 			} else {
// 				centerText("▄███", (row * 2), offset + (i * 2));
// 				centerText("▀███", (row * 2) + 1, offset + (i * 2));
// 			}
// 		} else if (i == size - 1) {
// 			if (vertical) {
// 				centerText("████", (row * 2) + (i * 2), offset);
// 				centerText("▀██▀", (row * 2) + (i * 2) + 1, offset);
// 			} else {
// 				centerText("███▄", (row * 2), offset + (i * 2));
// 				centerText("███▀", (row * 2) + 1, offset + (i * 2));
// 			}
// 		} else {
// 			if (vertical) {
// 				centerText("████", (row * 2) + (i * 2), offset);
// 				centerText("████", (row * 2) + (i * 2) + 1, offset);
// 			} else {
// 				centerText("████", (row * 2), offset + (i * 2));
// 				centerText("████", (row * 2) + 1, offset + (i * 2));
// 			}
// 		}
// 	}
// }

// void drawBomb(int row, int offset, int y, int x) {
// 	setColor(BACKGROUND_BLUE | BACKGROUND_INTENSITY);

// 	row += -GRID + (y - 1);

// 	offset += -(GRID * 2) + 2 + ((x - 1) * 4);

// 	centerText(" ▄▄ ", row, offset);
// 	centerText(" ▀▀ ", row + 1, offset);
// }