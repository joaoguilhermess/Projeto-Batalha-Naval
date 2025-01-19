#include "rules.h"

void rules() {
	clearConsole();

	while (true) {
		adjustConsole();

		std::string input = getInput();

		if (input == "exit") {
			return;
		}

		drawRules();

		delay();
	}
}

void drawRules() {
	drawText("REGRAS", 60, {
		"O jogo Batalha Naval é composto por dois tabuleiros com 10",
		"linhas e 10 colunas, um tabuleiro para cada jogador.",
		"Os tabuleiros são identificados por coordenadas expressadas",
		"por letras e números.",
		"Cada navio ocupa um número de quadrados consecutivos no",
		"tabuleiro, posicionados horizontalmente ou verticalmente.",
		"Os tipos e quantidades de navios são os mesmos para ambos os",
		"jogadores.",
		"Os jogadores não podem ver os navios uns dos outros.",
		"Para o jogo começar, cada jogador posiciona seus navios em",
		"seu respectivo tabuleiro, e assim a série de rodadas é",
		"iniciada.",
		"Em cada rodada, cada jogador tem um turno para atacar uma",
		"coordenada do tabuleiro adversário.",
		"Apenas quando todas as coordenadas ocupadas por um navio são",
		"atingidas o navio é destruido.",
		"Se todos os navios de um jogador forem destruidos o jogo",
		"acaba e o adversário vence."
	});
}