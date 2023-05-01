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

		bool posto1Encontrado = false; // flag para verificar se o posto 1 foi encontrado

		for (int i = 0; i < linhas; i++) {
			getline(file, line);
			std::vector<char> row;
			for (int j = 0; j < colunas; j++) {
				if (j < line.size()) {
					row.push_back(line[j]);

					if (!posto1Encontrado && line[j] == '1') {
						start = std::make_pair(i, j);
						std::cout << "Start: (" << i << ", " << j << ")" << std::endl;
						posto1Encontrado = true;
					}
					else if (isdigit(line[j]) && line[j] != '0') {
						int target;
						std::stringstream(line.substr(j, 1)) >> target;
						targets.push_back(std::make_pair(i, j));
					}
				}
			}
			mapa[i] = row;
		}

		std::cout << "Fui lido com sucesso :)" << std::endl;
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

// Erros: não está percorrendo por ordem
int Reader::bfs() {

	int targets_count = targets.size();
	int current_target = 1;
	int current_moves = 0;
	int linha_movimento[] = { -1, 0, 1, 0 };
	int coluna_movimento[] = { 0, 1, 0, -1 };

	std::vector<std::vector<bool>> percorrido(linhas, std::vector<bool>(colunas, false));
	std::queue<std::pair<int, int>> fila;

	fila.push(start);
	percorrido[start.first][start.second] = true;

	while (!fila.empty()) {
		int tamanho_fila = fila.size(); // número de elementos na fila antes de entrar no loop
		for (int i = 0; i < tamanho_fila; i++) {
			std::pair<int, int> atual = fila.front();
			fila.pop();

			std::cout << "Estou no posto " << mapa[atual.first][atual.second] << std::endl;


			//O condicional abaixo serve para calcular se o próximo elemento
			//segue a ordem.
			if (mapa[atual.first][atual.second] == current_target % 9 + 1 + '0') {
				current_target++;
				if (current_target == targets_count + 1) {
					return current_moves;
				}
			}

			for (int j = 0; j < 4; j++) {
				int nova_linha = atual.first + linha_movimento[j];
				int nova_coluna = atual.second + coluna_movimento[j];

				if (is_valid(nova_linha, nova_coluna) && !percorrido[nova_linha][nova_coluna]) {
					percorrido[nova_linha][nova_coluna] = true;
					if (mapa[nova_linha][nova_coluna] != '*') {
						fila.push(std::make_pair(nova_linha, nova_coluna));
					}
				}
			}
		}

		current_moves++;
	}
}


int Reader::distancia() {
	std::pair<int, int> posicao_atual = start;
	int distancia_total = 0;
	int numero_atual = 1;
	while (numero_atual <= targets.size()) {
		std::pair<int, int> proximo_numero = targets[numero_atual - 1];
		int distancia = bfs();
		if (distancia == -1) {
			return -1; // Não foi possível alcançar o próximo número, então a distância total não pode ser calculada
		}
		distancia_total += distancia;
		posicao_atual = proximo_numero;
		numero_atual++;
	}
	std::cout << "Soma total: " << distancia_total << std::endl;
	return distancia_total;
}
