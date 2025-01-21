#include "console.h"

bool output = false;

void adjustConsole() {
	if (!output) {
		output = true;

		SetConsoleOutputCP(65001);
	}

	adjustFontSize();

	resizeBuffer();

	enableCursor(false);

	setFullscreen();
}

void adjustFontSize() {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_FONT_INFOEX info;

	info.cbSize = sizeof(info);

	GetCurrentConsoleFontEx(handle, false, &info);
	
	info.dwFontSize.X = 0;
	info.dwFontSize.Y = GetSystemMetrics(SM_CYSCREEN) / ROWS;

	wcscpy(info.FaceName, L"Consolas");

	SetCurrentConsoleFontEx(handle, false, &info);
}

void resizeBuffer() {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO info;

	GetConsoleScreenBufferInfo(handle, &info);
	
	COORD size;

	size.Y = info.srWindow.Bottom - info.srWindow.Top + 1;
	size.X = info.dwSize.X;

	SetConsoleScreenBufferSize(handle, size);
}

void enableCursor(bool enable) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO info;

	GetConsoleCursorInfo(handle, &info);

	info.bVisible = enable;

	SetConsoleCursorInfo(handle, &info);
}

void setCursor(int y, int x) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD position;

	position.Y = y;
	position.X = x;

	SetConsoleCursorPosition(handle, position);
}

bool fullscreen = false;

void setFullscreen() {
	if (!fullscreen) {
		fullscreen = true;

		GetSystemMetrics(SM_CYSCREEN);

		HWND window = GetConsoleWindow();

		ShowWindow(window, SW_MAXIMIZE);

		SendMessage(window, WM_SYSKEYDOWN, VK_RETURN, 0x20000000);
	}
}

void delay(long long elapsed) {
	int interval = 1000 / FPS;

	interval -= elapsed;

	if (interval < 0) {
		interval = 0;
	}

	Sleep(interval);
}

void delay() {
	Sleep(1000 / FPS);
}

int getRows() {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO info;

	GetConsoleScreenBufferInfo(handle, &info);

	return info.srWindow.Bottom - info.srWindow.Top + 1;
}

int getColumns() {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO info;

	GetConsoleScreenBufferInfo(handle, &info);

	return info.dwSize.X;
}

int getLength(std::string text) {
	int length = text.length();

	int result = 0;

	for (int i = 0; i < length; i++) {
		char c = text[i];

		result += 1;
		
		if (c < 0) {
			if (c == -61) {
				i += 1;
			} else {
				i += 2;
			}
		}
	}

	return result;
}

void clearConsole() {
	int length = ROWS * getColumns();

	std::string buffer;

	for (int i = 0; i < length; i++) {
		buffer += " ";
	}

	setCursor(0, 0);

	std::cout << buffer;
}

void setColor(WORD color) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(handle, color);
}

void leftText(std::string text, int row, int offset) {
	setCursor(row, offset);

	std::cout << text;
}

void centerText(std::string text, int row, int offset) {
	leftText(text, row, (getColumns() / 2) - (getLength(text) / 2) + offset);
}

void rightText(std::string text, int row, int offset) {
	leftText(text, row, getColumns() - text.length() + offset);
}

void centerBorder(int rows, int columns, int row, int offset) {
	std::string line;

	for (int i = 0; i < columns; i++) {
		line += "─";
	}

	row -= (rows / 2) + 1;

	centerText("╔═" + line + "═╗", row, offset); row += 1;

	for (int i = 0; i < rows; i++) {
		centerText("│ ", row, offset - (1 + (columns / 2)));
		centerText(" │", row, offset + (1 + ((columns + 1) / 2)));

		row += 1;
	}

	centerText("╚═" + line + "═╝", row, offset);
}

void drawButton(std::string text, int row, int offset, bool selected) {
	int length = getLength(text);

	if (selected) {
		setColor(LIGHT_TEXT_COLOR);
	} else {
		setColor(DARK_TEXT_COLOR);
	}

	centerBorder(1, length, row, offset);

	if (selected) {
		setColor(LIGHT_TEXT_COLOR);
	} else {
		setColor(TEXT_COLOR);
	}

	centerText(text, row, offset);
}

void drawText(std::string title, int columns, std::vector<std::string> lines) {
	int length = lines.size();

	int middle = ROWS / 2;

	int row = middle - (length / 2);

	setColor(TEXT_COLOR);

	centerBorder(length + 2, columns, middle, 0);

	setColor(LIGHT_TEXT_COLOR);

	centerText(title, row - 1, 0);

	for (int i = 0; i < length; i++) {
		centerText(lines[i], row + i + 1, 0);
	}
}

std::string getInput() {
	if (_kbhit()) {
		char c = getch();
	
		if (c == 65 || c == 75 || c == 97) {
			return "left";
		}

		if (c == 68 || c == 77 || c == 100) {
			return "right";
		}

		if (c == 72 || c == 87 || c == 119) {
			return "up";
		}

		if (c == 80 || c == 83 || c == 115) {
			return "down";
		}

		if (c == 13 || c == 32) {
			return "enter";
		}

		if (c == 27) {
			return "exit";
		}

		if (c > 0) {
			setColor(LIGHT_TEXT_COLOR);

			leftText(std::to_string(c) + "   ", 0, 0);
		}
	}

	return "";
}