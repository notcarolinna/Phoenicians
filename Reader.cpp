#include "Reader.h"
#include <fstream>
#include <string>
#include <sstream>

Reader::Reader(const std::string& filename) : arquivo(filename), linhas(0), colunas(0) {}

// Precisa começar do valor 1;
// A primeira linha contém o tamanho de linhas e colunas, respectivamente;
// . é um caminho e * é um obstáculo;
// Caso um posto esteja obstruído, ignorá-lo e ir ao próximo

// Caso o posto 1 esteja obstruído, começar do 2. Ou fazer uma verificação para o programa
// começar a partir do primeiro posto não obstruído seguindo a ordem cardinal

void Reader::read() {
	std::ifstream file(arquivo);
	std::string line;

	if (file.is_open()) {
		getline(file, line);
		std::stringstream(line) >> linhas;
		std::stringstream(line.substr(line.find(' ') + 1)) >> colunas;

		// Verificação para saber se o número de linhas e colunas
		// foram obtidos corretamente
		std::cout << "Linhas: " << linhas << "\nColunas: " << colunas << std::endl;

		mapa = std::vector<std::vector<char>>(linhas, std::vector<char>(colunas));

		start = std::make_pair(0, 0); // primeiro posto da sequência não obstruído
		int firstTarget = 1; // primeiro alvo da sequência não obstruído

		for (int i = 0; i < linhas; i++) {
			getline(file, line);
			std::vector<char> row;
			for (int j = 0; j < colunas; j++) {
				if (j < line.size()) {
					row.push_back(line[j]);

					if (line[j] >= '1' && line[j] <= '9') {
						int num;
						std::stringstream(line.substr(j, 1)) >> num;

						if (num == firstTarget) {
							start = std::make_pair(i, j);
							std::cout << "Coordenadas do primeiro posto da sequencia: (" << i << ", " << j << ")" << std::endl;
							firstTarget++;
						}
						targets.push_back(std::make_pair(i, j));
					}
				}
			}
			mapa[i] = row;
		}

		if (firstTarget == 1) {
			std::cerr << "Nao foi encontrado nenhum posto da sequência 1-9." << std::endl;
			return;
		}

		std::cout << "Fui lido com sucesso :)\n" << std::endl;
		file.close();
	}
	else {
		std::cerr << "Nao fui lido com sucesso :(" << std::endl;
	}
}


// isso aqui tá certo
bool Reader::is_valid(int i, int j) {
	if (i < 0 || i >= linhas || j < 0 || j >= colunas) {
		return false;
	}
	if (mapa[i][j] == '*') {
		return false;
	}

	return true;
}

int Reader::bfs() {
	int targets_count = targets.size();
	int current_target = 1;
	int current_moves = 0;

	int linha_movimento[] = { -1, 0, 1, 0 }; // -1: cima, 0: fica, 1: baixo
	int coluna_movimento[] = { 0, 1, 0, -1 }; // 1: direita, 0: fica, -1: direita

	std::vector<std::vector<bool>> percorrido(linhas, std::vector<bool>(colunas, false));
	std::queue<std::pair<int, int>> fila;
	fila.push(start);
	percorrido[start.first][start.second] = true;

	while (current_target <= targets_count && current_moves <= 9) {
		int size = fila.size();
		for (int i = 0; i < size; i++) {
			std::pair<int, int> vertex = fila.front();
			fila.pop();
			int linha = vertex.first;
			int coluna = vertex.second;

			for (int j = 0; j < 4; j++) {
				int nova_linha = linha + linha_movimento[j];
				int nova_coluna = coluna + coluna_movimento[j];

				if (is_valid(nova_linha, nova_coluna) && !percorrido[nova_linha][nova_coluna]) {
					percorrido[nova_linha][nova_coluna] = true;
					fila.push(std::make_pair(nova_linha, nova_coluna));
					if (std::find(targets.begin(), targets.end(), std::make_pair(nova_linha, nova_coluna)) != targets.end()) {
						if (current_target == targets_count) {
							return current_moves + 1;
						}
						current_target++;
						current_moves += (nova_linha - linha) + (nova_coluna - coluna);
						std::cout << "Valor lido: " << mapa[nova_linha][nova_coluna] << ", coordenadas: " << nova_linha << "," << nova_coluna << std::endl;
						fila = std::queue<std::pair<int, int>>();
						fila.push(std::make_pair(nova_linha, nova_coluna));
						std::vector<std::vector<bool>> novo_percorrido(linhas, std::vector<bool>(colunas, false));
						novo_percorrido[nova_linha][nova_coluna] = true;
						percorrido = novo_percorrido;
						break;
					}
					else {
						fila.push(std::make_pair(nova_linha, nova_coluna));
						percorrido[nova_linha][nova_coluna] = true;
					}
				}
			}
		}
	}
	return -1; // Indica que não foi possível encontrar uma solução
}
