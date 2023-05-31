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
	std::unordered_map<int, std::pair<int, int>> mapa_coordenadas; // Mapa de coordenadas
	std::pair<int, int> partida; // Coordenadas de partida

public:
	Dados(std::string arquivo) : arquivo(arquivo) {}
	void Mapa();
	void DFS();
	int DistanciaAbsoluta(const std::pair<int, int>& origem, const std::pair<int, int>& destino);
	int CalculaDistancia(const std::pair<int, int>& origem, const std::pair<int, int>& destino);
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
				mapa_coordenadas.emplace(num, std::make_pair(i, j));
			}
		}
		grafo[i] = col;
	}
}

int Dados::DistanciaAbsoluta(const std::pair<int, int>& origem, const std::pair<int, int>& destino) {
	return abs(origem.first - destino.first) + abs(origem.second - destino.second);
}

int Dados::CalculaDistancia(const std::pair<int, int>& origem, const std::pair<int, int>& destino) {
	std::vector<std::vector<bool>> mapa_visitado(linhas, std::vector<bool>(colunas, false));
	std::vector<std::vector<int>> mapa_combustivel(linhas, std::vector<int>(colunas, linhas * colunas)); // valor máximo
	int combustivel_gasto = 0;

	mapa_visitado[origem.first][origem.second] = true; // Marca o vértice inicial como visitado
	mapa_combustivel[origem.first][origem.second] = combustivel_gasto;

	std::priority_queue<std::pair<int, std::pair<int, int>>> procurando;
	procurando.emplace(std::make_pair(0, origem));

	while (!procurando.empty()) {
		std::pair<int, int> atual = procurando.top().second;
		procurando.pop();

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
				if (combustivel_gasto < mapa_combustivel[linha][coluna]) {
					mapa_combustivel[linha][coluna] = combustivel_gasto;
					procurando.emplace(std::make_pair(-(DistanciaAbsoluta(vizinho, destino) + mapa_combustivel[vizinho.first][vizinho.second]), vizinho));
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

		std::pair<int, int> destino = mapa_coordenadas[atual];
		std::cout << "Posto " << atual << " na coordenada (" << destino.first << "," << destino.second << ")" << std::endl;
		int distancia = CalculaDistancia(partida, destino);

		if (distancia == -1) {
			std::cout << "Porto obstruido" << std::endl;
		}
		else {
			std::cout << "Distancia: " << distancia << std::endl;
			combustivel += distancia;
			partida = destino;
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
