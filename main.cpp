#include "main.h"

int main() {
	int selected = 1;

	clearConsole();

	while (true) {
		adjustConsole();

		int row = 0;

		drawTitle(row);

		std::string input = getInput();

		if (input == "left") {
			if (selected > 0) {
				selected -= 1;
			}
		} else if (input == "right") {
			if (selected < 2) {
				selected += 1;
			}
		} else if (input == "enter") {
			if (selected == 0) {
				rules();
			} else if (selected == 1) {
				game();
			} else if (selected == 2) {
				credits();
			}
		} else if (input == "exit") {
			break;
		}

		drawButtons(row, selected);

		delay();
	}

	return 0;
}

void drawTitle(int &row) {
	setColor(TITLE_COLOR);

	centerText(" ███████████             █████              ████  █████               ", row, 0); row += 1;
	centerText("░░███░░░░░███           ░░███              ░░███ ░░███                ", row, 0); row += 1;
	centerText(" ░███    ░███  ██████   ███████    ██████   ░███  ░███████    ██████  ", row, 0); row += 1;
	centerText(" ░██████████  ░░░░░███ ░░░███░    ░░░░░███  ░███  ░███░░███  ░░░░░███ ", row, 0); row += 1;
	centerText(" ░███░░░░░███  ███████   ░███      ███████  ░███  ░███ ░███   ███████ ", row, 0); row += 1;
	centerText(" ░███    ░███ ███░░███   ░███ ███ ███░░███  ░███  ░███ ░███  ███░░███ ", row, 0); row += 1;
	centerText(" ███████████ ░░████████  ░░█████ ░░████████ █████ ████ █████░░████████", row, 0); row += 1;
	centerText("░░░░░░░░░░░   ░░░░░░░░    ░░░░░   ░░░░░░░░ ░░░░░ ░░░░ ░░░░░  ░░░░░░░░ ", row, 0); row += 1;

	row += 1;

	centerText(" ██████   █████                                 ████ ", row, 0); row += 1;
	centerText("░░██████ ░░███                                 ░░███ ", row, 0); row += 1;
	centerText(" ░███░███ ░███   ██████   █████ █████  ██████   ░███ ", row, 0); row += 1;
	centerText(" ░███░░███░███  ░░░░░███ ░░███ ░░███  ░░░░░███  ░███ ", row, 0); row += 1;
	centerText(" ░███ ░░██████   ███████  ░███  ░███   ███████  ░███ ", row, 0); row += 1;
	centerText(" ░███  ░░█████  ███░░███  ░░███ ███   ███░░███  ░███ ", row, 0); row += 1;
	centerText(" █████  ░░█████░░████████  ░░█████   ░░████████ █████", row, 0); row += 1;
	centerText("░░░░░    ░░░░░  ░░░░░░░░    ░░░░░     ░░░░░░░░ ░░░░░ ", row, 0); row += 1;
}

void drawButtons(int &row, int selected) {
	int middle = row + ((ROWS - 1) - row) / 2;

	int offset = getColumns() / 4;

	drawButton("REGRAS", middle, -offset, selected == 0);

	drawButton("JOGAR", middle, 0, selected == 1);
	
	drawButton("CRÉDITOS", middle, offset, selected == 2);
}