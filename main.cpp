#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include "conio.h"

#define TEXTO_AZUL FOREGROUND_BLUE | FOREGROUND_INTENSITY

#define TEXTO_CLARO FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY

#define TEXTO FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED

#define TEXTO_ESCURO FOREGROUND_INTENSITY

#define TEXTO_VERMELHO BACKGROUND_BLUE | BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_INTENSITY

#define TEXTO_VERDE BACKGROUND_BLUE | BACKGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_INTENSITY

#define TEXTO_AZUL_ESCURO BACKGROUND_BLUE | BACKGROUND_INTENSITY | FOREGROUND_BLUE

#define FUNDO_AZUL BACKGROUND_BLUE | BACKGROUND_INTENSITY

#define QUANTIDADE_NAVIOS 4

#define TAMANHO_TABULEIRO 10

using namespace std;

string pegarInput() {
	if (_kbhit()) {
		char tecla = getch();

		if (tecla == 65 || tecla == 75 || tecla == 97) {
			return "esquerda";
		}

		if (tecla == 68 || tecla == 77 || tecla == 100) {
			return "direita";
		}

		if (tecla == 72 || tecla == 87 || tecla == 119) {
			return "cima";
		}

		if (tecla == 80 || tecla == 83 || tecla == 115) {
			return "baixo";
		}

		if (tecla == 13 || tecla == 32) {
			return "ok";
		}

		if (tecla == 27) {
			return "sair";
		}
	}

	return "";
}

void moverCursor(int linha, int coluna) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD posição;

	posição.Y = linha;
	posição.X = coluna;

	SetConsoleCursorPosition(console, posição);

	CONSOLE_CURSOR_INFO info;

	GetConsoleCursorInfo(console, &info);

	info.bVisible = false;

	SetConsoleCursorInfo(console, &info);
}

void trocarCor(int color) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(console, color);
}

int linhasTela() {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO info;

	GetConsoleScreenBufferInfo(console, &info);

	return info.srWindow.Bottom - info.srWindow.Top + 1;
}

int colunasTela() {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO info;

	GetConsoleScreenBufferInfo(console, &info);

	return info.dwSize.X;
}

int ultimasLinhas = 0;
int ultimasColunas = 0;

void limparConsole(bool forçar) {
	int linhas = linhasTela();
	int colunas = colunasTela();

	trocarCor(0);

	if (linhas != ultimasLinhas || colunas != ultimasColunas || forçar) {
		ultimasLinhas = linhas;
		ultimasColunas = colunas;

		int tamanho = linhas * colunas;

		string borracha;

		for (int i = 0; i < tamanho; i++) {
			borracha += " ";
		}

		moverCursor(0, 0);

		cout << borracha;
	}
}

int tamanhoTexto(string texto) {
	int tamanho = texto.length();

	int resultado = 0;

	for (int i = 0; i < tamanho; i++) {
		char caractere = texto[i];

		resultado += 1;

		if (caractere < 0) {
			if (caractere == -61) {
				i += 1;
			} else {
				i += 2;
			}
		}
	}

	return resultado;
}

void escreverTextoEsquerda(string texto, int linha, int coluna) {
	if (texto == "") {
		return;
	}

	moverCursor(linha, coluna);

	cout << texto;
}

void escreverTextoMeio(string texto, int linha, int coluna) {
	escreverTextoEsquerda(texto, linha, coluna - (tamanhoTexto(texto) / 2));
}

void escreverTextoMeio(vector<string> linhas, int& linha, int coluna) {
	int quantidade = linhas.size();

	for (int i = 0; i < quantidade; i++) {
		escreverTextoMeio(linhas[i], linha, coluna);

		linha += 1;
	}
}

void desenharBorda(int altura, int largura, int linha, int coluna) {
	linha -= 1;

	string barra;

	for (int i = 0; i < largura; i++) {
		barra += "─";
	}

	escreverTextoMeio("╔═" + barra + "═╗", linha, coluna); linha += 1;

	for (int i = 0; i < altura; i++) {
		escreverTextoEsquerda("│", linha, coluna - (2 + (largura / 2)));
		escreverTextoEsquerda("│", linha, coluna + (2 + ((largura - 1) / 2)));

		linha += 1;
	}

	escreverTextoMeio("╚═" + barra + "═╝", linha, coluna);
}

void desenharBotão(string texto, bool selecionado, int linha, int coluna) {
	if (selecionado) {
		trocarCor(TEXTO_CLARO);
	} else {
		trocarCor(TEXTO_ESCURO);
	}

	desenharBorda(1, tamanhoTexto(texto), linha, coluna);

	if (selecionado) {
		trocarCor(TEXTO_CLARO);
	} else {
		trocarCor(TEXTO);
	}

	escreverTextoMeio(texto, linha, coluna);
}

int calcularEspaçamentoVertical(int linhasUsadas, int blocos) {
	return (linhasTela() - linhasUsadas) / (blocos + 1);
}

int calcularEspaçamentoHorizontal(int colunasUsadas, int blocos) {
	return (colunasTela() - colunasUsadas) / (blocos + 1);
}

void mostrarRegras() {
	limparConsole(true);

	bool rodando = true;

	while (rodando) {
		string input = pegarInput();

		if (input == "ok") {
			rodando = false;
		} else if (input == "sair") {
			rodando = false;
		}

		limparConsole(false);

		moverCursor(0, 0);

		int alturaTexto = 23;

		int espaçamentoVertical = calcularEspaçamentoVertical(alturaTexto, 1);
		
		int linha = espaçamentoVertical;
		int colunaMeio = colunasTela() / 2;

		trocarCor(TEXTO);

		desenharBorda(alturaTexto, 95, linha, colunaMeio);

		escreverTextoMeio("REGRAS", linha, colunaMeio);

		linha += 2;

		trocarCor(TEXTO_CLARO);

		escreverTextoMeio({
			"O jogo Batalha Naval é composto por dois tabuleiros com 10 linhas e 10 colunas, um tabuleiro",
			"para cada jogador.",
			"",
			"Os tabuleiros são identificados por coordenadas expressadas por letras e números.",
			"",
			"Cada navio ocupa um número de quadrados consecutivos no tabuleiro, posicionados horizontalmente",
			"ou verticalmente.",
			"",
			"Os tipos e quantidades de navios são os mesmos para ambos os jogadores.",
			"",
			"Os jogadores não podem ver os navios uns dos outros.",
			"",
			"Para o jogo começar, cada jogador posiciona seus navios em seu respectivo tabuleiro, e assim a",
			"série de rodadas é iniciada.",
			"",
			"Em cada rodada, cada jogador tem um turno para atacar uma coordenada do tabuleiro adversário.",
			"",
			"Apenas quando todas as coordenadas ocupadas por um navio são atingidas o navio é destruido.",
			"",
			"Se todos os navios de um jogador forem destruidos o jogo acaba e o adversário vence.",
			""
		}, linha, colunaMeio);

		trocarCor(TEXTO_ESCURO);

		escreverTextoMeio("ok: [ENTER, ESPAÇO] sair: [ESC]", linhasTela() - 2, colunaMeio);
	}
}

struct Bomba {
	int linha;
	int coluna;
	bool selecionada;
};

struct Navio {
	int linha;
	int coluna;
	int tamanho;
	bool vertical;
	bool selecionado;
};

struct Jogador {
	string nome;
	vector<Navio> navios;
	vector<Bomba> bombas;
	string lado;
};

struct Célula {
	bool temÁgua;
	bool temBomba;
	bool temNavio;
	bool temColisão;
	bool tamanhoNavio;
	bool posiçãoNavio;
	bool vertical;
	bool selecionada;
};

void limparTabuleiro(array<array<Célula, TAMANHO_TABULEIRO>, TAMANHO_TABULEIRO>& tabuleiro) {
	for (int l = 0; l < TAMANHO_TABULEIRO; l++) {
		for (int c = 0; c < TAMANHO_TABULEIRO; c++) {
			Célula& célula = tabuleiro[l][c];

			célula.temÁgua = true;
			célula.temBomba = false;
			célula.temNavio = false;
			célula.temColisão = false;
			célula.tamanhoNavio = 0;
			célula.posiçãoNavio = 0;
			célula.vertical = false;
			célula.selecionada = false;
		}
	}
}

void desenharTabuleiro(array<array<Célula, TAMANHO_TABULEIRO>, TAMANHO_TABULEIRO>& tabuleiro, int linha, int coluna) {
	trocarCor(TEXTO_CLARO);

	escreverTextoEsquerda("A   B   C   D   E   F   G   H   I   J", linha, coluna + 3);

	linha += 1;

	for (int l = 0; l < TAMANHO_TABULEIRO; l++) {
		escreverTextoMeio(to_string(l + 1), linha + (l * 2), coluna);
	}

	coluna += 2;

	for (int l = 0; l < TAMANHO_TABULEIRO; l++) {
		for (int c = 0; c < TAMANHO_TABULEIRO; c++) {
			Célula& célula = tabuleiro[l][c];

			string parte1;
			string parte2;

			if (célula.temNavio) {
				if (célula.temColisão) {
					trocarCor(TEXTO_VERMELHO);
				} else if (célula.selecionada) {
					trocarCor(TEXTO_VERDE);
				} else if (célula.temBomba) {
					trocarCor(FUNDO_AZUL);
				} else {
					trocarCor(TEXTO_ESCURO);
				}

				parte1 = "████";
				parte2 = "████";
			} else if (célula.temBomba) {
				if (célula.temColisão) {
					trocarCor(TEXTO_VERMELHO);
				} else if (célula.selecionada) {
					trocarCor(TEXTO_VERDE);
				} else if (célula.temÁgua) {
					trocarCor(TEXTO_AZUL_ESCURO);
				} else {
					trocarCor(FUNDO_AZUL);
				}

				parte1 = " ▄▄ ";
				parte2 = " ▀▀ ";
			} else {
				trocarCor(FUNDO_AZUL);

				parte1 = "    ";
				parte2 = "    ";
			}

			escreverTextoEsquerda(parte1, linha + (l * 2), coluna + (c * 4));
			escreverTextoEsquerda(parte2, linha + 1 + (l * 2), coluna + (c * 4));
		}
	}
}

void colocarNavios(vector<Navio>& navios, array<array<Célula, TAMANHO_TABULEIRO>, TAMANHO_TABULEIRO>& tabuleiro, int& colisõesNavio, bool desenharNavios) {
	colisõesNavio = 0;

	int quantidade = navios.size();

	for (int n = 0; n < quantidade; n++) {
		Navio& navio = navios[n];

		for (int i = 0; i < navio.tamanho; i++) {
			Célula* célula;

			if (navio.vertical) {
				célula = &tabuleiro[navio.linha + i][navio.coluna];
			} else {
				célula = &tabuleiro[navio.linha][navio.coluna + i];
			}

			if (célula->temNavio || célula->temBomba) {
				colisõesNavio += 1;
			}

			if (célula->temNavio) {
				célula->temColisão = true;
			}

			if (desenharNavios) {
				célula->temÁgua = false;
				célula->temNavio = true;
				célula->tamanhoNavio = navio.tamanho;
				célula->posiçãoNavio = i;
				célula->vertical = navio.vertical;
				célula->selecionada = navio.selecionado;
			} else {
				célula->temÁgua = false;
			}
		}
	}
}

void moverNavio(Jogador& jogador, Navio& navio, array<array<Célula, TAMANHO_TABULEIRO>, TAMANHO_TABULEIRO>& tabuleiro, bool& rodando) {
	if (!rodando) {
		return;
	}

	bool girandoNavio = true;

	bool movendoNavio = false;

	int colisõesNavio = 0;

	while (rodando && (girandoNavio || movendoNavio)) {
		string input = pegarInput();

		if (input == "sair") {
			rodando = false;

			girandoNavio = false;

			movendoNavio = false;
		} else if (girandoNavio) {
			if (input == "esquerda") {
				navio.vertical = true;
			} else if (input == "direita") {
				navio.vertical = false;
			} else if (input == "ok") {
				girandoNavio = false;

				movendoNavio = true;
			}
		} else if (movendoNavio) {
			if (input == "cima") {
				if (navio.linha > 0) {
					navio.linha -= 1;
				}
			} else if (input == "baixo") {
				int tamanho = 1;

				if (navio.vertical) {
					tamanho = navio.tamanho;
				}

				if (navio.linha < TAMANHO_TABULEIRO - tamanho) {
					navio.linha += 1;
				}
			} else if (input == "esquerda") {
				if (navio.coluna > 0) {
					navio.coluna -= 1;
				}
			} else if (input == "direita") {
				int tamanho = 1;

				if (!navio.vertical) {
					tamanho = navio.tamanho;
				}

				if (navio.coluna < TAMANHO_TABULEIRO - tamanho) {
					navio.coluna += 1;
				}
			} else if (input == "ok") {
				if (colisõesNavio == 0) {
					movendoNavio = false;
				}
			}
		}

		limparConsole(false);

		moverCursor(0, 0);

		int colunaMeio = colunasTela() / 2;

		int espaçamentoVertical = calcularEspaçamentoVertical(1 + 20 + 1, 1);

		int espaçamentoHorizontal = calcularEspaçamentoHorizontal(2 + 40 + 2 + 40, 2);

		int linha = espaçamentoVertical;
		int coluna1 = espaçamentoHorizontal;
		int coluna2 = espaçamentoHorizontal + 42 + espaçamentoHorizontal;

		trocarCor(TEXTO_CLARO);

		if (girandoNavio) {
			escreverTextoMeio("O " + jogador.nome + " Deve Girar O Navio", linha / 2, colunaMeio);
		} else {
			escreverTextoMeio("O " + jogador.nome + " Deve Mover O Navio", linha / 2, colunaMeio);
		}

		limparTabuleiro(tabuleiro);

		colocarNavios(jogador.navios, tabuleiro, colisõesNavio, true);

		if (jogador.lado == "esquerdo") {
			desenharTabuleiro(tabuleiro, linha, coluna1);
		} else {
			desenharTabuleiro(tabuleiro, linha, coluna2);
		}

		limparTabuleiro(tabuleiro);

		if (jogador.lado == "esquerdo") {
			desenharTabuleiro(tabuleiro, linha, coluna2);
		} else {
			desenharTabuleiro(tabuleiro, linha, coluna1);
		}

		trocarCor(TEXTO_ESCURO);

		if (girandoNavio) {
			escreverTextoMeio("                 esquerda: [ESQUERDA, A] direita: [DIREITA, D] ok: [ENTER, ESPAÇO] sair: [ESC]                 ", linhasTela() - 2, colunaMeio);
		} else {
			escreverTextoMeio("cima: [CIMA, W] baixo: [BAIXO, S] esquerda: [ESQUERDA, A] direita: [DIREITA, D] ok: [ENTER, ESPAÇO] sair: [ESC]", linhasTela() - 2, colunaMeio);
		}
	}
}

void moverNavios(Jogador& jogador, array<array<Célula, TAMANHO_TABULEIRO>, TAMANHO_TABULEIRO>& tabuleiro, bool& rodando) {
	if (!rodando) {
		return;
	}

	for (int n = 0; n < QUANTIDADE_NAVIOS; n++) {
		Navio navioTemporário;

		navioTemporário.linha = 0;
		navioTemporário.coluna = 0;

		navioTemporário.tamanho = QUANTIDADE_NAVIOS - n + 1;

		navioTemporário.vertical = true;

		jogador.navios.push_back(navioTemporário);

		Navio& navio = jogador.navios[jogador.navios.size() - 1];

		navio.selecionado = true;

		moverNavio(jogador, navio, tabuleiro, rodando);

		navio.selecionado = false;
	}
}

void colocarBombas(vector<Bomba>& bombas, array<array<Célula, TAMANHO_TABULEIRO>, TAMANHO_TABULEIRO>& tabuleiro, bool& colisãoBomba) {
	colisãoBomba = false;

	int quantidade = bombas.size();

	for (int b = 0; b < quantidade; b++) {
		Bomba& bomba = bombas[b];

		Célula& célula = tabuleiro[bomba.linha][bomba.coluna];

		if (célula.temBomba) {
			colisãoBomba = true;

			célula.temColisão = true;
		}

		célula.temBomba = true;

		célula.selecionada = bomba.selecionada;
	}
}

void jogarTurno(Jogador& jogador, Jogador& adversário, Jogador& vencedor, array<array<Célula, TAMANHO_TABULEIRO>, TAMANHO_TABULEIRO>& tabuleiro, bool& rodando, int célulasNavio) {
	if (!rodando) {
		return;
	}

	Bomba bombaTemporária;

	bombaTemporária.linha = 0;
	bombaTemporária.coluna = 0;

	jogador.bombas.push_back(bombaTemporária);

	Bomba& bomba = jogador.bombas[jogador.bombas.size() - 1];

	bool movendoBomba = true;

	bool colisãoBomba = false;

	int colisõesNavio = 0;

	bomba.selecionada = true;

	while (rodando && movendoBomba) {
		string input = pegarInput();

		if (input == "sair") {
			rodando = false;

			movendoBomba = false;
		} else if (input == "cima") {
			if (bomba.linha > 0) {
				bomba.linha -= 1;
			}
		} else if (input == "baixo") {
			if (bomba.linha < TAMANHO_TABULEIRO - 1) {
				bomba.linha += 1;
			}
		} else if (input == "esquerda") {
			if (bomba.coluna > 0) {
				bomba.coluna -= 1;
			}
		} else if (input == "direita") {
			if (bomba.coluna < TAMANHO_TABULEIRO - 1) {
				bomba.coluna += 1;
			}
		} else if (input == "ok") {
			if (!colisãoBomba) {
				movendoBomba = false;
			}
		}

		limparConsole(false);

		moverCursor(0, 0);

		int colunaMeio = colunasTela() / 2;

		int espaçamentoVertical = calcularEspaçamentoVertical(1 + 20 + 1, 1);

		int espaçamentoHorizontal = calcularEspaçamentoHorizontal(2 + 40 + 2 + 40, 2);

		int linha = espaçamentoVertical;
		int coluna1 = espaçamentoHorizontal;
		int coluna2 = espaçamentoHorizontal + 42 + espaçamentoHorizontal;

		trocarCor(TEXTO_CLARO);

		escreverTextoMeio("O " + jogador.nome + " Deve Colocar Uma Bomba No Tabuleiro Do " + adversário.nome, linha / 2, colunaMeio);

		limparTabuleiro(tabuleiro);

		colocarBombas(adversário.bombas, tabuleiro, colisãoBomba);

		colocarNavios(jogador.navios, tabuleiro, colisõesNavio, true);

		if (jogador.lado == "esquerdo") {
			desenharTabuleiro(tabuleiro, linha, coluna1);
		} else {
			desenharTabuleiro(tabuleiro, linha, coluna2);
		}

		limparTabuleiro(tabuleiro);

		colocarBombas(jogador.bombas, tabuleiro, colisãoBomba);

		colocarNavios(adversário.navios, tabuleiro, colisõesNavio, false);

		if (jogador.lado == "esquerdo") {
			desenharTabuleiro(tabuleiro, linha, coluna2);
		} else {
			desenharTabuleiro(tabuleiro, linha, coluna1);
		}

		if (colisõesNavio == célulasNavio && !movendoBomba) {
			rodando = false;

			vencedor = adversário;
		}

		trocarCor(TEXTO_ESCURO);

		escreverTextoMeio("cima: [CIMA, W] baixo: [BAIXO, S] esquerda: [ESQUERDA, A] direita: [DIREITA, D] ok: [ENTER, ESPAÇO] sair: [ESC]", linhasTela() - 2, colunasTela() / 2);
	}

	bomba.selecionada = false;
}

void mostrarVencedor(Jogador& jogador, Jogador& adversário, Jogador& vencedor, array<array<Célula, TAMANHO_TABULEIRO>, TAMANHO_TABULEIRO>& tabuleiro) {
	limparConsole(true);

	bool rodando = true;

	bool colisãoBomba = false;

	int colisõesNavio = 0;

	while (rodando) {
		string input = pegarInput();

		if (input == "ok") {
			rodando = false;
		} else if (input == "sair") {
			rodando = false;
		}

		limparConsole(false);

		moverCursor(0, 0);

		int colunaMeio = colunasTela() / 2;

		int espaçamentoVertical = calcularEspaçamentoVertical(1 + 20 + 1, 1);

		int espaçamentoHorizontal = calcularEspaçamentoHorizontal(2 + 40 + 2 + 40, 2);

		int linha = espaçamentoVertical;
		int coluna1 = espaçamentoHorizontal;
		int coluna2 = espaçamentoHorizontal + 42 + espaçamentoHorizontal;

		trocarCor(TEXTO_CLARO);

		escreverTextoMeio(jogador.nome + " Vencedor", linha / 2, colunaMeio);

		limparTabuleiro(tabuleiro);

		colocarBombas(adversário.bombas, tabuleiro, colisãoBomba);

		colocarNavios(jogador.navios, tabuleiro, colisõesNavio, true);

		if (jogador.lado == "esquerdo") {
			desenharTabuleiro(tabuleiro, linha, coluna1);
		} else {
			desenharTabuleiro(tabuleiro, linha, coluna2);
		}

		limparTabuleiro(tabuleiro);

		colocarBombas(jogador.bombas, tabuleiro, colisãoBomba);

		colocarNavios(adversário.navios, tabuleiro, colisõesNavio, true);

		if (jogador.lado == "esquerdo") {
			desenharTabuleiro(tabuleiro, linha, coluna2);
		} else {
			desenharTabuleiro(tabuleiro, linha, coluna1);
		}

		trocarCor(TEXTO_ESCURO);

		escreverTextoMeio("ok: [ENTER, ESPAÇO] sair: [ESC]", linhasTela() - 2, colunasTela() / 2);
	}
}

void mostrarJogo() {
	limparConsole(true);

	Jogador jogador1;
	Jogador jogador2;

	jogador1.nome = "Jogador 1";
	jogador2.nome = "Jogador 2";

	jogador1.navios = {};
	jogador2.navios = {};

	jogador1.lado = "esquerdo";
	jogador2.lado = "direito";

	array<array<Célula, TAMANHO_TABULEIRO>, TAMANHO_TABULEIRO> tabuleiro;

	int célulasNavio = 0;

	for (int n = 0; n < QUANTIDADE_NAVIOS; n++) {
		célulasNavio += QUANTIDADE_NAVIOS - n + 1;
	}

	bool rodando = true;

	moverNavios(jogador1, tabuleiro, rodando);
	moverNavios(jogador2, tabuleiro, rodando);

	Jogador vencedor;

	vencedor.nome = "";

	while (rodando) {
		jogarTurno(jogador1, jogador2, vencedor, tabuleiro, rodando, célulasNavio);
		jogarTurno(jogador2, jogador1, vencedor, tabuleiro, rodando, célulasNavio);
	}

	if (vencedor.nome != "") {
		mostrarVencedor(jogador1, jogador2, vencedor, tabuleiro);
	}
}

void mostrarCreditos() {
	limparConsole(true);

	bool rodando = true;

	while (rodando) {
		string input = pegarInput();

		if (input == "ok") {
			rodando = false;
		} else if (input == "sair") {
			rodando = false;
		}

		limparConsole(false);

		moverCursor(0, 0);

		int alturaTexto = 18;

		int espaçamentoVertical = calcularEspaçamentoVertical(alturaTexto, 1);

		int linha = espaçamentoVertical;
		int colunaMeio = colunasTela() / 2;

		trocarCor(TEXTO);

		desenharBorda(alturaTexto, 42, linha, colunaMeio);

		escreverTextoMeio("CRÉDITOS", linha, colunaMeio);

		linha += 2;

		trocarCor(TEXTO_CLARO);

		escreverTextoMeio({
			"Ariele De Carvalho Mendonça",
			"",
			"Guilherme Augusto Seixas De Oliveira Bispo",
			"",
			"Izaque Dos Santos",
			"",
			"João Guilherme Santos Silva",
			"",
			"Julia Santa Rosa Santana",
			"",
			"Samuel Barros Silva",
			"",
			"Ulisses Dorenski Lima Ribeiro",
			"",
			"Wallace Gabriel Santos De Jesus",
			""
		}, linha, colunaMeio);

		trocarCor(TEXTO_ESCURO);

		escreverTextoMeio("ok: [ENTER, ESPAÇO] sair: [ESC]", linhasTela() - 2, colunaMeio);
	}
}

int main() {
	SetConsoleOutputCP(65001);

	limparConsole(true);

	bool rodando = true;

	int selecionado = 1;

	while (rodando) {
		string input = pegarInput();

		if (input == "esquerda") {
			if (selecionado > 0) {
				selecionado -= 1;
			}	
		} else if (input == "direita") {
			if (selecionado < 2) {
				selecionado += 1;
			}	
		} else if (input == "ok") {
			switch(selecionado) {
				case 0:
					mostrarRegras();
					
					break;
				case 1:
					mostrarJogo();

					break;
				case 2:
					mostrarCreditos();

					break;
			}

			limparConsole(true);
		} else if (input == "sair") {
			rodando = false;
		}
	
		limparConsole(false);

		moverCursor(0, 0);

		int espaçamentoVertical = calcularEspaçamentoVertical(8 + 1 + 8 + 1, 2);

		int linha = espaçamentoVertical;
		int colunaMeio = colunasTela() / 2;

		trocarCor(TEXTO_AZUL);

		escreverTextoMeio({
			" ███████████             █████              ████  █████               ",
			"░░███░░░░░███           ░░███              ░░███ ░░███                ",
			" ░███    ░███  ██████   ███████    ██████   ░███  ░███████    ██████  ",
			" ░██████████  ░░░░░███ ░░░███░    ░░░░░███  ░███  ░███░░███  ░░░░░███ ",
			" ░███░░░░░███  ███████   ░███      ███████  ░███  ░███ ░███   ███████ ",
			" ░███    ░███ ███░░███   ░███ ███ ███░░███  ░███  ░███ ░███  ███░░███ ",
			" ███████████ ░░████████  ░░█████ ░░████████ █████ ████ █████░░████████",
			"░░░░░░░░░░░   ░░░░░░░░    ░░░░░   ░░░░░░░░ ░░░░░ ░░░░ ░░░░░  ░░░░░░░░ ",
			"",
			" ██████   █████                                 ████ ",
			"░░██████ ░░███                                 ░░███ ",
			" ░███░███ ░███   ██████   █████ █████  ██████   ░███ ",
			" ░███░░███░███  ░░░░░███ ░░███ ░░███  ░░░░░███  ░███ ",
			" ░███ ░░██████   ███████  ░███  ░███   ███████  ░███ ",
			" ░███  ░░█████  ███░░███  ░░███ ███   ███░░███  ░███ ",
			" █████  ░░█████░░████████  ░░█████   ░░████████ █████",
			"░░░░░    ░░░░░  ░░░░░░░░    ░░░░░     ░░░░░░░░ ░░░░░ "
		}, linha, colunaMeio);

		linha += espaçamentoVertical;

		int espaçamentoHorizontal = calcularEspaçamentoHorizontal(0, 3);

		int coluna = espaçamentoHorizontal;

		desenharBotão("REGRAS", selecionado == 0, linha, coluna);

		coluna += espaçamentoHorizontal;
		
		desenharBotão("JOGAR", selecionado == 1, linha, coluna);

		coluna += espaçamentoHorizontal;

		desenharBotão("CRÉDITOS", selecionado == 2, linha, coluna);

		trocarCor(TEXTO_ESCURO);

		escreverTextoMeio("esquerda: [ESQUERDA, A] direita: [DIREITA, D] ok: [ENTER, ESPAÇO] sair: [ESC]", linhasTela() - 2, colunaMeio);
	}

	return 0;
}