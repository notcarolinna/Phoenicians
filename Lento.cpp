#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <list>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <iomanip>
#include <chrono>


class Dados {
private:
	int linhas;
	int colunas;
	std::string arquivo;

	std::vector<std::vector<char>> grafo; // Armazena o grafo
	std::vector<std::pair<int, int>> coordenadas; // Armazena as coordenadas dos portos
	std::pair<int, int> partida; // Coordenadas de partida

public:
	Dados(std::string arquivo) : arquivo(arquivo) {}
	void Mapa();
	void DFS();
	int DistanciaAbsoluta(std::pair<int, int> origem, std::pair<int, int> destino);
	int CalculaDistancia(std::pair<int, int> origem, std::pair<int, int> destino);
};

void Dados::Mapa() {
	std::ifstream file("./resources/" + arquivo);

	if (!file.is_open()) {
		std::cerr << "Falha ao abrir o arquivo " << arquivo << std::endl;
		return;
	}

	std::string line;
	std::getline(file, line);
	std::stringstream ss(line);
	ss >> linhas >> colunas;

	std::cout << "\n\nLinhas: " << linhas << "\nColunas: " << colunas << std::endl;

	grafo = std::vector<std::vector<char>>(linhas, std::vector<char>(colunas));

	int portoAtual = 1;

	for (int i = 0; i < linhas; i++) {
		std::getline(file, line);
		std::vector<char> col(colunas, ' ');

		for (int j = 0; j < colunas && j < line.size(); j++) {
			col[j] = line[j];

			if (line[j] >= '1' && line[j] <= '9') { // Se o caractere for um número de 1 até 9
				int num = line[j] - '0';
				if (num == portoAtual) {
					partida = { i, j };
				}
				coordenadas.emplace_back(i, j);
			}
		}

		grafo[i] = col;
	}

	std::cout << "\nCoordenadas dos portos:\n";
	for (const auto& coord : coordenadas) {
		std::cout << "(" << coord.first << "," << coord.second << ")" << std::endl;
	}

	std::cout << "\nPartida:\n";
	std::cout << "(" << partida.first << "," << partida.second << ")" << std::endl;
}

int Dados::DistanciaAbsoluta(std::pair<int, int> origem, std::pair<int, int> destino) {
	return abs(origem.first - destino.first) + abs(origem.second - destino.second);
}

int Dados::CalculaDistancia(std::pair<int, int> origem, std::pair<int, int> destino) {
	std::vector<std::vector<bool>> mapa_visitado(linhas, std::vector<bool>(colunas, false));
	std::vector<std::vector<int>> mapa_combustivel(linhas, std::vector<int>(colunas, linhas * colunas)); // valor máximo
	int combustivel_gasto = 0;

	mapa_visitado[origem.first][origem.second] = true; // Marca o vértice inicial como visitado
	mapa_combustivel[origem.first][origem.second] = combustivel_gasto;

	std::list<std::pair<int, int>> procurando;
	procurando.push_back(origem);

	while (!procurando.empty()) {
		std::pair<int, int> atual = { -1, -1 };
		int combustivel_estimado = linhas * colunas; // valor máximo

		for (const auto& posicao : procurando) { // pega a coordenada com menor combustivel estimado
			if (DistanciaAbsoluta(posicao, destino) + mapa_combustivel[posicao.first][posicao.second] < combustivel_estimado) {
				combustivel_estimado = DistanciaAbsoluta(posicao, destino) + mapa_combustivel[posicao.first][posicao.second];
				atual = posicao;
			}
		}
		procurando.remove(atual);

		int i = atual.first;
		int j = atual.second;
		mapa_visitado[i][j] = true; // ja foi checado

		if (i == destino.first && j == destino.second) {
			return mapa_combustivel[i][j];
		}

		combustivel_gasto = mapa_combustivel[i][j] + 1;

		std::vector<std::pair<int, int>> vizinhos = { {i - 1, j}, {i + 1, j}, {i, j + 1}, {i, j - 1} };

		for (const auto& vizinho : vizinhos) {
			int linha = vizinho.first;
			int coluna = vizinho.second;


			if (linha >= 0 && linha < linhas && coluna >= 0 && coluna < colunas &&
				!mapa_visitado[linha][coluna] && grafo[linha][coluna] != '*') {
				if (combustivel_gasto < mapa_combustivel[linha][coluna] ||
					(std::find(procurando.begin(), procurando.end(), std::make_pair(linha, coluna)) == procurando.end())) {
					mapa_combustivel[linha][coluna] = combustivel_gasto;
					if (std::find(procurando.begin(), procurando.end(), std::make_pair(linha, coluna)) == procurando.end()) {
						procurando.push_back({ linha, coluna });
					}
				}
			}
		}
	}
	return -1;
}

void Dados::DFS() {
	int combustivel = 0;

	std::stack<int> portos;
	portos.push(1);
	for (int i = 9; i >= 2; i--) {
		portos.push(i);
	}

	while (!portos.empty()) {
		int atual = portos.top();
		portos.pop();

		for (auto& destino : coordenadas) {
			if (grafo[destino.first][destino.second] == atual + '0') {
				std::cout << "Posto " << atual << " na coordenada (" << destino.first << "," << destino.second << ")" << std::endl;
				//std::cout << "Partida: " << partida.first << "," << partida.second << std::endl;
				//std::cout << "Chegada: " << destino.first << "," << destino.second << std::endl;

				int distancia = CalculaDistancia(partida, destino);

				if (distancia == -1) {
					std::cout << "Porto obstruido" << std::endl;
				}
				else {
					std::cout << "Distancia: " << distancia << std::endl;
					combustivel += distancia;
					partida = destino;
				}
				break;
			}
		}
	}
	std::cout << "\nDistancia total percorrida: " << combustivel << std::endl;
}

int main() {

	auto start = std::chrono::high_resolution_clock::now();

	std::cout << "Caso 08" << std::endl;
	Dados dados("caso08.txt");

	dados.Mapa();

	std::cout << "\n\n" << std::endl;

	dados.DFS();

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

	std::cout << std::fixed << std::setprecision(1) << duration.count() << " segundos." << std::endl;

	return 0;
}
