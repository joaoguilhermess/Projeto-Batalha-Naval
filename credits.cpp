#include "credits.h"

void credits() {
	clearConsole();

	while (true) {
		adjustConsole();

		std::string input = getInput();

		if (input == "exit") {
			return;
		}

		drawCredits();

		delay();
	}
}

void drawCredits() {
	drawText("CRÉDITOS", 60, {
		"Ariele De Carvalho Mendonça",
		"Guilherme Augusto Seixas De Oliveira Bispo",
		"Izaque Dos Santos",
		"João Guilherme Santos Silva",
		"Julia Santa Rosa Santana",
		"Samuel Barros Silva",
		"Ulisses Dorenski Lima Ribeiro",
		"Wallace Gabriel Santos De Jesus"
	});
}