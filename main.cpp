#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
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

	COORD posicao;

	posicao.Y = linha;
	posicao.X = coluna;

	SetConsoleCursorPosition(console, posicao);

	CONSOLE_CURSOR_INFO info;

	GetConsoleCursorInfo(console, &info);

	info.bVisible = false;

	SetConsoleCursorInfo(console, &info);
}

void trocarCor(int cor) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(console, cor);
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

void limparConsole(bool forcar) {
	int linhas = linhasTela();
	int colunas = colunasTela();

	trocarCor(0);

	if (linhas != ultimasLinhas || colunas != ultimasColunas || forcar) {
		ultimasLinhas = linhas;
		ultimasColunas = colunas;

		string borracha(linhas * colunas, ' ');

		moverCursor(1, 1);

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
	if (texto == "") return;

	moverCursor(linha, coluna);

	cout << texto;
}

void escreverTextoMeio(string texto, int linha, int coluna) {
	escreverTextoEsquerda(texto, linha, coluna - (tamanhoTexto(texto) / 2));
}

void escreverTextoMeio(vector<string> linhas, int& linha, int coluna) {
	for (string& texto : linhas) {
		escreverTextoMeio(texto, linha, coluna);

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

void desenharBotao(string texto, bool selecionado, int linha, int coluna) {
	if (selecionado) trocarCor(TEXTO_CLARO);
	else trocarCor(TEXTO_ESCURO);

	desenharBorda(1, tamanhoTexto(texto), linha, coluna);

	if (selecionado) trocarCor(TEXTO_CLARO);
	else trocarCor(TEXTO);

	escreverTextoMeio(texto, linha, coluna);
}

int calcularEspacamentoVertical(int linhasUsadas, int blocos) {
	return (linhasTela() - linhasUsadas) / (blocos + 1);
}

int calcularEspacamentoHorizontal(int colunasUsadas, int blocos) {
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

		moverCursor(1, 1);

		int alturaTexto = 23;

		int espacamentoVertical = calcularEspacamentoVertical(alturaTexto, 1);
		
		int linha = espacamentoVertical;
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
			"Em cada rodada, cada jogador tem um turno para atacar uma coordenada do tabuleiro adversario.",
			"",
			"Apenas quando todas as coordenadas ocupadas por um navio são atingidas o navio é destruido.",
			"",
			"Se todos os navios de um jogador forem destruidos o jogo acaba e o adversario vence.",
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
	bool ladoEsquerdo;
	int acertos;
	int ataques;
	int partidasVencidas;
	int partidasJogadas;
};

struct Celula {
	bool temAgua;
	bool temBomba;
	bool temNavio;
	bool temColisao;
	bool tamanhoNavio;
	bool posicaoNavio;
	bool vertical;
	bool selecionada;
};

void limparTabuleiro(array<array<Celula, TAMANHO_TABULEIRO>, TAMANHO_TABULEIRO>& tabuleiro) {
	for (int l = 0; l < TAMANHO_TABULEIRO; l++) {
		for (int c = 0; c < TAMANHO_TABULEIRO; c++) {
			Celula& celula = tabuleiro[l][c];

			celula.temAgua = true;
			celula.temBomba = false;
			celula.temNavio = false;
			celula.temColisao = false;
			celula.tamanhoNavio = 0;
			celula.posicaoNavio = 0;
			celula.vertical = false;
			celula.selecionada = false;
		}
	}
}

void desenharTabuleiro(array<array<Celula, TAMANHO_TABULEIRO>, TAMANHO_TABULEIRO>& tabuleiro, int linha, int coluna) {
	trocarCor(TEXTO_CLARO);

	escreverTextoEsquerda("A   B   C   D   E   F   G   H   I   J", linha, coluna + 3);

	linha += 1;

	for (int l = 0; l < TAMANHO_TABULEIRO; l++) {
		escreverTextoMeio(to_string(l + 1), linha + (l * 2), coluna);
	}

	coluna += 2;

	for (int l = 0; l < TAMANHO_TABULEIRO; l++) {
		for (int c = 0; c < TAMANHO_TABULEIRO; c++) {
			Celula& celula = tabuleiro[l][c];

			string parte1;
			string parte2;

			if (celula.temNavio) {
				if (celula.temColisao) {
					trocarCor(TEXTO_VERMELHO);
				} else if (celula.selecionada) {
					trocarCor(TEXTO_VERDE);
				} else if (celula.temBomba) {
					trocarCor(FUNDO_AZUL);
				} else {
					trocarCor(TEXTO_ESCURO);
				}

				parte1 = "████";
				parte2 = "████";
			} else if (celula.temBomba) {
				if (celula.temColisao) {
					trocarCor(TEXTO_VERMELHO);
				} else if (celula.selecionada) {
					trocarCor(TEXTO_VERDE);
				} else if (celula.temAgua) {
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

void colocarNavios(vector<Navio>& navios, array<array<Celula, TAMANHO_TABULEIRO>, TAMANHO_TABULEIRO>& tabuleiro, int& colisoesNavio, bool desenharNavios) {
	colisoesNavio = 0;

	for (Navio& navio : navios) {
		for (int i = 0; i < navio.tamanho; i++) {
			Celula* celula;

			if (navio.vertical) celula = &tabuleiro[navio.linha + i][navio.coluna];
			else celula = &tabuleiro[navio.linha][navio.coluna + i];

			if (celula->temNavio || celula->temBomba) colisoesNavio += 1;

			if (celula->temNavio) celula->temColisao = true;

			if (desenharNavios) {
				celula->temAgua = false;
				celula->temNavio = true;
				celula->tamanhoNavio = navio.tamanho;
				celula->posicaoNavio = i;
				celula->vertical = navio.vertical;
				celula->selecionada = navio.selecionado;
			} else {
				celula->temAgua = false;
			}
		}
	}
}

void mostrarEstatisticas(Jogador& jogador, int linha, int coluna) {
	linha -= 1;

	escreverTextoMeio({
		jogador.nome,
		"Vencidas: " + to_string(jogador.partidasVencidas) + " | Jogadas: " + to_string(jogador.partidasJogadas),
		"Acertos: " + to_string(jogador.acertos) + " | Tentativas: " + to_string(jogador.ataques)
	}, linha, coluna);
}

void moverNavio(Jogador& jogador, Jogador& adversario, Navio& navio, array<array<Celula, TAMANHO_TABULEIRO>, TAMANHO_TABULEIRO>& tabuleiro, bool& rodando) {
	bool girandoNavio = true;

	bool movendoNavio = false;

	int colisoesNavio = 0;

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
			if (input == "cima" && navio.linha > 0) {
				navio.linha -= 1;
			} else if (input == "baixo" && navio.linha < TAMANHO_TABULEIRO - (navio.vertical ? navio.tamanho : 1)) {
				navio.linha += 1;
			} else if (input == "esquerda" && navio.coluna > 0) {
				navio.coluna -= 1;
			} else if (input == "direita" && navio.coluna < TAMANHO_TABULEIRO - (!navio.vertical ? navio.tamanho : 1)) {
				navio.coluna += 1;
			} else if (input == "ok" && colisoesNavio == 0) {
				movendoNavio = false;
			}
		}

		limparConsole(false);

		moverCursor(1, 1);

		int colunaMeio = colunasTela() / 2;

		int espacamentoVertical = calcularEspacamentoVertical(20, 1);
		int espacamentoHorizontal = calcularEspacamentoHorizontal(40 + 40, 2);

		int linha = espacamentoVertical;
		int coluna1 = espacamentoHorizontal;
		int coluna2 = espacamentoHorizontal + 40 + espacamentoHorizontal;

		trocarCor(TEXTO_CLARO);

		escreverTextoMeio("Vez do " + jogador.nome, espacamentoVertical / 2, colunaMeio);

		mostrarEstatisticas(jogador, espacamentoVertical / 2, (jogador.ladoEsquerdo ? coluna1 : coluna2) + (40 / 2));
		mostrarEstatisticas(adversario, espacamentoVertical / 2, (adversario.ladoEsquerdo ? coluna1 : coluna2) + (40 / 2));

		escreverTextoMeio("O " + jogador.nome + " Deve " + (girandoNavio ? "Girar" : "Mover") + " O Navio", espacamentoVertical + 20 + espacamentoVertical / 2, colunaMeio);

		limparTabuleiro(tabuleiro);

		colocarNavios(jogador.navios, tabuleiro, colisoesNavio, true);

		desenharTabuleiro(tabuleiro, linha, jogador.ladoEsquerdo ? coluna1 : coluna2);

		limparTabuleiro(tabuleiro);

		desenharTabuleiro(tabuleiro, linha, jogador.ladoEsquerdo ? coluna2 : coluna1);

		trocarCor(TEXTO_ESCURO);

		if (girandoNavio) escreverTextoMeio("                 esquerda: [ESQUERDA, A] direita: [DIREITA, D] ok: [ENTER, ESPAÇO] sair: [ESC]                 ", linhasTela() - 2, colunaMeio);
		else escreverTextoMeio("cima: [CIMA, W] baixo: [BAIXO, S] esquerda: [ESQUERDA, A] direita: [DIREITA, D] ok: [ENTER, ESPAÇO] sair: [ESC]", linhasTela() - 2, colunaMeio);
	}
}

void moverNavios(Jogador& jogador, Jogador& adversario, array<array<Celula, TAMANHO_TABULEIRO>, TAMANHO_TABULEIRO>& tabuleiro, bool& rodando) {
	if (!rodando) {
		return;
	}

	for (int n = 0; n < QUANTIDADE_NAVIOS; n++) {
		Navio navioTemporario;

		navioTemporario.linha = 0;
		navioTemporario.coluna = 0;

		navioTemporario.tamanho = QUANTIDADE_NAVIOS - n + 1;

		navioTemporario.vertical = true;

		jogador.navios.push_back(navioTemporario);

		Navio& navio = jogador.navios[jogador.navios.size() - 1];

		navio.selecionado = true;

		moverNavio(jogador, adversario, navio, tabuleiro, rodando);

		navio.selecionado = false;
	}
}

void colocarBombas(vector<Bomba>& bombas, array<array<Celula, TAMANHO_TABULEIRO>, TAMANHO_TABULEIRO>& tabuleiro, bool& colisaoBomba) {
	colisaoBomba = false;

	for (Bomba& bomba : bombas) {
		Celula& celula = tabuleiro[bomba.linha][bomba.coluna];

		if (celula.temBomba) {
			colisaoBomba = true;

			celula.temColisao = true;
		}

		celula.temBomba = true;

		celula.selecionada = bomba.selecionada;
	}
}

void jogarTurno(Jogador& jogador, Jogador& adversario, Jogador& vencedor, array<array<Celula, TAMANHO_TABULEIRO>, TAMANHO_TABULEIRO>& tabuleiro, bool& rodando, int celulasNavio) {
	if (!rodando) {
		return;
	}

	Bomba bombaTemporaria = {0, 0, true};

	jogador.bombas.push_back(bombaTemporaria);

	Bomba& bomba = jogador.bombas.back();

	bool movendoBomba = true;

	bool colisaoBomba = false;

	int colisoesNavio = 0;

	bomba.selecionada = true;

	while (rodando && movendoBomba) {
		string input = pegarInput();

		if (input == "sair") {
			rodando = false;

			movendoBomba = false;
		} else if (input == "cima" && bomba.linha > 0) {
			bomba.linha -= 1;
		} else if (input == "baixo" && bomba.linha < TAMANHO_TABULEIRO - 1) {
			bomba.linha += 1;
		} else if (input == "esquerda" && bomba.coluna > 0) {
			bomba.coluna -= 1;
		} else if (input == "direita" && bomba.coluna < TAMANHO_TABULEIRO - 1) {
			bomba.coluna += 1;
		} else if (input == "ok" && !colisaoBomba) {
			movendoBomba = false;

			jogador.ataques = jogador.bombas.size();
			jogador.acertos = colisoesNavio;
		}

		limparConsole(false);

		moverCursor(1, 1);

		int colunaMeio = colunasTela() / 2;

		int espacamentoVertical = calcularEspacamentoVertical(20, 1);
		int espacamentoHorizontal = calcularEspacamentoHorizontal(40 + 40, 2);

		int linha = espacamentoVertical;
		int coluna1 = espacamentoHorizontal;
		int coluna2 = espacamentoHorizontal + 40 + espacamentoHorizontal;

		trocarCor(TEXTO_CLARO);

		escreverTextoMeio("Vez do " + jogador.nome, espacamentoVertical / 2, colunaMeio);

		mostrarEstatisticas(jogador, espacamentoVertical / 2, (jogador.ladoEsquerdo ? coluna1 : coluna2) + (40 / 2));
		mostrarEstatisticas(adversario, espacamentoVertical / 2, (adversario.ladoEsquerdo ? coluna1 : coluna2) + (40 / 2));

		escreverTextoMeio("O " + jogador.nome + " Deve Colocar Uma Bomba No Tabuleiro Do " + adversario.nome, espacamentoVertical + 20 + espacamentoVertical / 2, colunaMeio);

		limparTabuleiro(tabuleiro);

		colocarBombas(adversario.bombas, tabuleiro, colisaoBomba);

		colocarNavios(jogador.navios, tabuleiro, colisoesNavio, true);

		desenharTabuleiro(tabuleiro, linha, jogador.ladoEsquerdo ? coluna1 : coluna2);

		limparTabuleiro(tabuleiro);

		colocarBombas(jogador.bombas, tabuleiro, colisaoBomba);

		colocarNavios(adversario.navios, tabuleiro, colisoesNavio, false);

		desenharTabuleiro(tabuleiro, linha, jogador.ladoEsquerdo ? coluna2 : coluna1);

		trocarCor(TEXTO_ESCURO);

		escreverTextoMeio("cima: [CIMA, W] baixo: [BAIXO, S] esquerda: [ESQUERDA, A] direita: [DIREITA, D] ok: [ENTER, ESPAÇO] sair: [ESC]", linhasTela() - 2, colunasTela() / 2);

		if (colisoesNavio == celulasNavio && !movendoBomba) {
			rodando = false;

			vencedor = jogador;

			jogador.partidasVencidas += 1;

			jogador.partidasJogadas += 1;
			adversario.partidasJogadas += 1;
		}
	}

	bomba.selecionada = false;
}

void mostrarVencedor(Jogador& jogador1, Jogador& jogador2, Jogador& vencedor, array<array<Celula, TAMANHO_TABULEIRO>, TAMANHO_TABULEIRO>& tabuleiro) {
	limparConsole(true);

	bool rodando = true;

	bool colisaoBomba = false;

	int colisoesNavio = 0;

	while (rodando) {
		string input = pegarInput();

		if (input == "ok") {
			rodando = false;
		} else if (input == "sair") {
			rodando = false;
		}

		limparConsole(false);

		moverCursor(1, 1);

		int colunaMeio = colunasTela() / 2;

		int espacamentoVertical = calcularEspacamentoVertical(20, 1);

		int espacamentoHorizontal = calcularEspacamentoHorizontal(40 + 40, 2);

		int linha = espacamentoVertical;
		int coluna1 = espacamentoHorizontal;
		int coluna2 = espacamentoHorizontal + 40 + espacamentoHorizontal;

		trocarCor(TEXTO_CLARO);

		escreverTextoMeio("O " + vencedor.nome + " Venceu de " + to_string(vencedor.acertos) + " a " + to_string((vencedor.nome == jogador1.nome ? jogador2 : jogador1).acertos) + "!", espacamentoVertical / 2, colunaMeio);

		mostrarEstatisticas(jogador1, espacamentoVertical / 2, (jogador1.ladoEsquerdo ? coluna1 : coluna2) + (40 / 2));
		mostrarEstatisticas(jogador2, espacamentoVertical / 2, (jogador2.ladoEsquerdo ? coluna1 : coluna2) + (40 / 2));

		limparTabuleiro(tabuleiro);

		colocarBombas(jogador2.bombas, tabuleiro, colisaoBomba);

		colocarNavios(jogador1.navios, tabuleiro, colisoesNavio, true);

		desenharTabuleiro(tabuleiro, linha, jogador1.ladoEsquerdo ? coluna1 : coluna2);

		limparTabuleiro(tabuleiro);

		colocarBombas(jogador1.bombas, tabuleiro, colisaoBomba);

		colocarNavios(jogador2.navios, tabuleiro, colisoesNavio, true);

		desenharTabuleiro(tabuleiro, linha, jogador1.ladoEsquerdo ? coluna2 : coluna1);

		trocarCor(TEXTO_ESCURO);

		escreverTextoMeio("ok: [ENTER, ESPAÇO] sair: [ESC]", linhasTela() - 2, colunasTela() / 2);
	}
}

void lerArquivo(string nome, Jogador& jogador) {
	ifstream arquivo;

	arquivo.open(nome, ios::in);

	if (!arquivo.is_open()) return;

	string linha;

	while (getline(arquivo, linha)) {
		size_t coluna1 = linha.find(jogador.nome);
		if (coluna1 == string::npos) continue;

		size_t coluna2 = linha.find(";", coluna1 + 1);
		if (coluna2 == string::npos) continue;

		size_t coluna3 = linha.find(";", coluna2 + 1);
		if (coluna3 == string::npos) continue;

		jogador.partidasVencidas = stoi(linha.substr(coluna2 + 1, coluna3 - (coluna2 + 1)));
		jogador.partidasJogadas = stoi(linha.substr(coluna3 + 1));
	}
}

void salvarArquivo(string nome, Jogador& jogador1, Jogador& jogador2) {
	ofstream arquivo;

	arquivo.open(nome, ios::out | ios::trunc);

	if (!arquivo.is_open()) return;

	arquivo << "Jogador;Partidas Vencidas;Partidas Jogadas\n";
	arquivo << jogador1.nome << ";" << jogador1.partidasVencidas << ";" << jogador1.partidasJogadas << "\n";
	arquivo << jogador2.nome << ";" << jogador2.partidasVencidas << ";" << jogador2.partidasJogadas << "\n";

	arquivo.close();
}

void mostrarJogo() {
	limparConsole(true);

	Jogador jogador1 = {"Jogador 1", {}, {}, true, 0, 0};
	Jogador jogador2 = {"Jogador 2", {}, {}, false, 0, 0};

	string arquivo = "dados.csv";

	lerArquivo(arquivo, jogador1);
	lerArquivo(arquivo, jogador2);

	salvarArquivo(arquivo, jogador1, jogador2);

	array<array<Celula, TAMANHO_TABULEIRO>, TAMANHO_TABULEIRO> tabuleiro;

	int celulasNavio = 0;

	for (int n = 0; n < QUANTIDADE_NAVIOS; n++) {
		celulasNavio += QUANTIDADE_NAVIOS - n + 1;
	}

	bool rodando = true;

	moverNavios(jogador1, jogador2, tabuleiro, rodando);
	moverNavios(jogador2, jogador1, tabuleiro, rodando);

	Jogador vencedor = {""};

	while (rodando) {
		jogarTurno(jogador1, jogador2, vencedor, tabuleiro, rodando, celulasNavio);
		jogarTurno(jogador2, jogador1, vencedor, tabuleiro, rodando, celulasNavio);

	}

	if (vencedor.nome != "") {
		salvarArquivo(arquivo, jogador1, jogador2);
		
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

		moverCursor(1, 1);

		int alturaTexto = 18;

		int espacamentoVertical = calcularEspacamentoVertical(alturaTexto, 1);

		int linha = espacamentoVertical;
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

	Jogador jogador1 = {"Jogador 1", {}, {}, true, 0, 0, 0, 0};
	Jogador jogador2 = {"Jogador 2", {}, {}, false, 0, 0, 0, 0};

	string arquivo = "dados.csv";

	lerArquivo(arquivo, jogador1);
	lerArquivo(arquivo, jogador2);

	limparConsole(true);

	bool rodando = true;

	int selecionado = 1;

	while (rodando) {
		string input = pegarInput();

		if (input == "esquerda" && selecionado > 0) {
			selecionado -= 1;
		} else if (input == "direita" && selecionado < 2) {
			selecionado += 1;
		} else if (input == "ok") {
			switch(selecionado) {
				case 0: mostrarRegras(); break;
				case 1: mostrarJogo(); break;
				case 2: mostrarCreditos(); break;
			}

			limparConsole(true);
		} else if (input == "sair") {
			rodando = false;
		}
	
		limparConsole(false);

		moverCursor(1, 1);

		int espacamentoVertical = calcularEspacamentoVertical(8 + 1 + 8 + 1, 2);

		int linha = espacamentoVertical;
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

		linha += espacamentoVertical;

		int espacamentoHorizontal = calcularEspacamentoHorizontal(0, 3);

		int coluna = espacamentoHorizontal;

		desenharBotao("REGRAS", selecionado == 0, linha, coluna);

		coluna += espacamentoHorizontal;
		
		desenharBotao("JOGAR", selecionado == 1, linha, coluna);

		coluna += espacamentoHorizontal;

		desenharBotao("CRÉDITOS", selecionado == 2, linha, coluna);

		trocarCor(TEXTO_ESCURO);

		escreverTextoMeio("esquerda: [ESQUERDA, A] direita: [DIREITA, D] ok: [ENTER, ESPAÇO] sair: [ESC]", linhasTela() - 2, colunaMeio);
	}

	return 0;
}