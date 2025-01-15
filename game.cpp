#include "game.h"

void game() {
	clearConsole();

	while(true) {
		adjustConsole();

		int offset = getColumns() / 4;

		int row = (ROWS / 2);

		drawGrid(row, -offset);

		drawBoat(row, -offset, 1, 1, 5, true);
		drawBoat(row, -offset, 1, 3, 4, true);
		drawBoat(row, -offset, 1, 5, 3, true);
		drawBoat(row, -offset, 1, 7, 2, true);
		drawBoat(row, -offset, 10, 1, 5, false);

		drawGrid(row, offset);

		std::string input = getInput();

		if (input == "exit") {
			return clearConsole();
		}

		delay();
	}
}

void drawGrid(int row, int offset) {
	row -= (GRID / 2);

	std::string grid;
	std::string grid2;

	for (int i = 0; i < GRID; i++) {
		grid += "▄▀";
	}

	setColor(TEXT_COLOR);

	centerText("A B C D E F G H I J ", row - 1, offset);

	for (int i = 0; i < GRID; i++) {
		setColor(TEXT_COLOR);
		
		centerText(std::to_string(i + 1), row, offset - (GRID + 2));

		// setColor(BACKGROUND_BLUE | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		setColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);

		centerText(grid, row, offset);

		row += 1;
	}

	setColor(BACKGROUND_COLOR);
}

void drawBoat(int row, int offset, int y, int x, int size, int vertical) {
	setColor(TEXT_COLOR);

	row += -(GRID / 2) + (y - 1);

	offset += -GRID + 1 + ((x - 1) * 2);

	for (int i = 0; i < size; i++) {
		if (vertical) {
			centerText("██", row + i, offset);
		} else {
			centerText("██", row, offset + (i * 2));
		}
	}
}