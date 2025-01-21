#include "game.h"

void game2() {
	clearConsole();

	std::array<Ship, SHIPS> leftShips = createShips();
	std::array<Ship, SHIPS> rightShips = createShips();

	std::array<std::array<Cell, GRID>, GRID> grid;

	int selectedShip = 0;

	bool isRotating = true;
	bool isMovingShip = true;

	bool collided = false;

	while (true) {
		std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();

		adjustConsole();

		Ship& ship = leftShips[selectedShip];

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

		clearGrid(grid);

		setShips(leftShips, grid, collided);

		int offset = getColumns() / 4;

		int row = (ROWS / 2);

		drawGrid(grid, row, -offset);

		std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();

		std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

		setColor(0);

		delay(duration.count());
	}
}

void game() {
	clearConsole();

	adjustConsole();

	Player player1;
	Player player2;

	player1.name = "Jogador 1";
	player2.name = "Jogador 2";

	player1.ships = createShips();
	player2.ships = createShips();

	std::array<std::array<Cell, GRID>, GRID> grid;

	int middle = (ROWS / 2);

	int offset = getColumns() / 4;

	bool running = true;

	moveShips(player1, grid, middle, offset, running);

	moveShips(player2, grid, middle, offset, running);

	// Player winner;

	// while (running) {
	// 	playTurn(player1, player2, grid, row, offset, running, winner);
	// 	playTurn(player2, player1, grid, row, offset, running, winner);
	// }

	// showWinner(winner);
}

void playTurn(Player& current, Player& other, std::array<std::array<Cell, GRID>, GRID>& grid, int row, int offset, bool& running, Player& winner) {
	if (!running) {
		return;
	}

	Bomb bomb;

	bomb.y = 0;
	bomb.x = 0;
	bomb.isSunked = false;

	bool collided = false;

	while (true) {
		while (true) {
			std::string input = getInput();

			if (input == "") {
				break;
			} else if (input == "exit") {
				running = false;

				return;
			} else if (input == "up") {
				if (bomb.y > 0) {
					bomb.y -= 1;
				}
			} else if (input == "down") {
				if (bomb.y < GRID - 1) {
					bomb.y += 1;
				}
			} else if (input == "left") {
				if (bomb.x > 0) {
					bomb.x -= 1;
				}
			} else if (input == "right") {
				if (bomb.x < GRID - 1) {
					bomb.x += 1;
				}
			} else if (input == "enter") {
				if (!collided) {
					current.bombs.push_back(bomb);

					return;
				}
			}
		}

		clearGrid(grid);

		// setBombs(other.bombs, grid);

		bool asd = false;

		setShips(current.ships, grid, asd);

		drawGrid(grid, row, -offset);
	}
}

void moveShips(Player& current, std::array<std::array<Cell, GRID>, GRID>& grid, int row, int offset, bool& running) {
	if (!running) {
		return;
	}

	setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

	centerText("Vez do: " + current.name, 1, 0);

	int selected = 0;

	bool isRotating = true;

	bool collided = false;

	while (selected < SHIPS) {
		Ship& ship = current.ships[selected];

		ship.isVisible = true;

		while (true) {
			std::string input = getInput();

			if (input == "") {
				break;
			} else if (input == "exit") {
				running = false;

				return;
			} else if (isRotating) {
				if (input == "left") {
					ship.isVertical = true;
				} else if (input == "right") {
					ship.isVertical = false;
				} else if (input == "enter") {
					isRotating = false;
				}
			} else {
				if (input == "up") {
					if (ship.y > 0) {
						ship.y -= 1;
					}
				} else if (input == "down") {
					int size = 1;

					if (ship.isVertical) {
						size = ship.size;
					}

					if (ship.y < GRID - size) {
						ship.y += 1;
					}
				} else if (input == "left") {
					if (ship.x > 0) {
						ship.x -= 1;
					}
				} else if (input == "right") {
					int size = 1;

					if (!ship.isVertical) {
						size = ship.size;
					}

					if (ship.x < GRID - size) {
						ship.x += 1;
					}
				} else if (input == "enter") {
					if (!collided) {
						selected += 1;

						isRotating = true;
					}
				}
			}
		}

		clearGrid(grid);

		drawGrid(grid, row, offset);

		setShips(current.ships, grid, collided);

		drawGrid(grid, row, -offset);
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
		ship.isSelected = false;
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

void setBombs(std::vector<Bomb>& bombs, std::array<std::array<Cell, GRID>, GRID>& grid) {
	int length = bombs.size();

	for (int b = 0; b < length; b++) {
		Bomb& bomb = bombs[b];

		Cell& cell = grid[bomb.y][bomb.x];

		cell.isWater = false;
		cell.hasBomb = true;
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

	setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

	centerText(" A   B   C   D   E   F   G   H   I   J  ", row - 1, offset);

	offset -= (GRID - 1) * 2;

	for (int r = 0; r < GRID; r++) {
		centerText(std::to_string(r + 1), row + (r * 2), offset - 4);
	}

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