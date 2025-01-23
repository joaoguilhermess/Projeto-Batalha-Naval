#include "game.h"

void game() {
	clearConsole();

	adjustConsole();

	Player player1;
	Player player2;

	player1.name = "Jogador 1";
	player2.name = "Jogador 2";

	player1.ships = createShips();
	player2.ships = createShips();

	int shipCells = 0;

	for (int i = 0; i < SHIPS; i++) {
		shipCells += SHIPS - i + 1;
	}

	std::array<std::array<Cell, GRID>, GRID> grid;

	int middle = (ROWS / 2);

	int offset = getColumns() / 4;

	bool running = true;

	moveShips(player1, grid, middle, offset, running);

	moveShips(player2, grid, middle, -offset, running);

	Player winner;

	winner.name = "";

	while (running) {
		playTurn(player1, player2, grid, middle, offset, running, shipCells, winner);
		playTurn(player2, player1, grid, middle, -offset, running, shipCells, winner);
	}

	if (winner.name != "") {
		showWinner(player1, player2, grid, middle, offset, winner);
	}
}

void setTitle(std::string title) {
	setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

	centerText(title, 1, 0);
}

void showWinner(Player& player1, Player& player2, std::array<std::array<Cell, GRID>, GRID>& grid, int row, int offset, Player& winner) {
	bool bombCollided = false;
	int shipCollided = 0;

	int index = 0;

	while (true) {
		setTitle("O " + winner.name + " Venceu");

		std::string input = getInput();

		if (input != "") {
			return;
		}

		leftText(std::to_string(player1.bombs.size()) + " " + std::to_string(player1.bombs[player1.bombs.size() - 1].y) + " " + std::to_string(player1.bombs[player1.bombs.size() - 1].x), 9, 0);
		leftText(std::to_string(player2.bombs.size()) + " " + std::to_string(player2.bombs[player2.bombs.size() - 1].y) + " " + std::to_string(player2.bombs[player2.bombs.size() - 1].x), 11, 0);
		leftText(std::to_string(winner.bombs.size()) + " " + std::to_string(winner.bombs[winner.bombs.size() - 1].y) + " " + std::to_string(winner.bombs[winner.bombs.size() - 1].x), 13, 0);

		clearGrid(grid);

		setBombs(player2.bombs, grid, bombCollided);

		// if (bombCollided) {
		// 	Bomb& bomb = player2.bombs[index];

		// 	leftText(std::to_string(index) + " " + std::to_string(bomb.y) + " " + std::to_string(bomb.x), 9, 0);

		// 	Sleep(1000 / 1);

		// 	index += 1;

		// 	if (index > player2.bombs.size() - 1) {
		// 		index = 0;
		// 	}
		// }

		setShips(player1.ships, grid, shipCollided);

		drawGrid(grid, row, -offset);

		clearGrid(grid);

		setBombs(player1.bombs, grid, bombCollided);

		setShips(player2.ships, grid, shipCollided);

		drawGrid(grid, row, offset);
	}
}

void playTurn(Player& current, Player& other, std::array<std::array<Cell, GRID>, GRID>& grid, int row, int offset, bool& running, int shipCells, Player& winner) {
	if (!running) {
		return;
	}

	Bomb temp;

	temp.y = 0;
	temp.x = 0;
	temp.isSunked = false;

	current.bombs.push_back(temp);

	Bomb& bomb = current.bombs[current.bombs.size() - 1];

	bool movingBomb = true;

	bool bombCollided = false;

	int shipCollided = 0;
	
	while (movingBomb) {
		setTitle("O " + current.name + " Deve Posicionar Uma Bomba no Tabuleiro do " + other.name + ".");

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
				if (!bombCollided) {
					movingBomb = false;

					break;
				}
			}
		}

		clearGrid(grid);

		setBombs(other.bombs, grid, bombCollided);

		setShips(current.ships, grid, shipCollided);

		drawGrid(grid, row, -offset);

		clearGrid(grid);

		setBombs(current.bombs, grid, bombCollided);

		drawGrid(grid, row, offset);

		if (shipCollided == shipCells) {
			running = false;

			winner = other;

			return;
		}
	}
}

void moveShips(Player& current, std::array<std::array<Cell, GRID>, GRID>& grid, int row, int offset, bool& running) {
	if (!running) {
		return;
	}

	int selected = 0;

	bool isRotating = true;

	int collided = 0;

	while (selected < SHIPS) {
		setTitle("O " + current.name + " Deve Posicionar Seus Navios.");

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
					if (collided == 0) {
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
			cell.shipSize = 0;
			cell.shipIndex = 0;
			cell.isVertical = false;
		}
	}
}

void setBombs(std::vector<Bomb>& bombs, std::array<std::array<Cell, GRID>, GRID>& grid, bool& collided) {
	collided = false;

	int length = bombs.size();

	for (int b = 0; b < length; b++) {
		Bomb& bomb = bombs[b];

		Cell& cell = grid[bomb.y][bomb.x];

		if (cell.hasBomb) {
			collided = true;

			cell.isCollided = true;
		}

		cell.isWater = false;
		cell.hasBomb = true;
	}
}

void setShips(std::array<Ship, SHIPS>& ships, std::array<std::array<Cell, GRID>, GRID>& grid, int& collided) {
	collided = 0;

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

				if (cell->hasShip || cell->hasBomb) {
					collided += 1;

				}
				if (cell->hasShip) {		
					cell->isCollided = true;
				}

				cell->isWater = false;
				cell->hasShip = true;
				cell->shipSize = ship.size;
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
				} else if (cell.hasBomb) {
					setColor(BACKGROUND_BLUE | BACKGROUND_INTENSITY);
				} else {
					setColor(BACKGROUND_BLUE | BACKGROUND_INTENSITY | FOREGROUND_INTENSITY);
				}
				
				centerText("████", row + (r * 2), offset + (c * 4));
				centerText("████", row + (r * 2) + 1, offset + (c * 4));

				// if (cell.shipIndex == 0) {
				// 	if (cell.isVertical) {
				// 		centerText("▄██▄", row + (r * 2), offset + (c * 4));
				// 		centerText("████", row + (r * 2) + 1, offset + (c * 4));
				// 	} else {
				// 		centerText("▄███", row + (r * 2), offset + (c * 4));
				// 		centerText("▀███", row + (r * 2) + 1, offset + (c * 4));
				// 	}
				// } else if (cell.shipIndex == cell.shipSize - 1) {
				// 	if (cell.isVertical) {
				// 		centerText("████", row + (r * 2), offset + (c * 4));
				// 		centerText("▀██▀", row + (r * 2) + 1, offset + (c * 4));
				// 	} else {
				// 		centerText("███▄", row + (r * 2), offset + (c * 4));
				// 		centerText("███▀", row + (r * 2) + 1, offset + (c * 4));
				// 	}
				// } else {	
				// 	if (cell.isVertical) {
				// 		centerText("████", row + (r * 2), offset + (c * 4));
				// 		centerText("████", row + (r * 2) + 1, offset + (c * 4));
				// 	} else {
				// 		centerText("████", row + (r * 2), offset + (c * 4));
				// 		centerText("████", row + (r * 2) + 1, offset + (c * 4));
				// 	}
				// }
			} else if (cell.hasBomb) {
				if (cell.isCollided) {
					setColor(BACKGROUND_BLUE | BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_INTENSITY);
				} else {
					setColor(BACKGROUND_BLUE | BACKGROUND_INTENSITY);
				}

				centerText(" ▄▄ ", row + (r * 2), offset + (c * 4));
				centerText(" ▀▀ ", row + (r * 2) + 1, offset + (c * 4));
			}
		}
	}
}