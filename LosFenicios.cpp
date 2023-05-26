#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>


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
	partida = { 0, 0 };

	int portoAtual = 1;

	for (int i = 0; i < linhas; i++) {

		std::getline(file, line);
		std::vector<char> col(colunas, ' ');

		for (int j = 0; j < colunas && j < line.size(); j++) {

			col[j] = line[j];

			//std::cout << "Lendo caractere " << line[j] << " na coordenada (" << i << "," << j << ")" << std::endl;

			if (line[j] >= '1' && line[j] <= '9') { // Se o caractere for um número de 1 até 9

				int atual = line[j] - '0';

				if (atual == portoAtual) {
					partida = { i, j };
					portoAtual++;
				}
				coordenadas.emplace_back(i, j);
			}
		}

		grafo[i] = col;
	}
}

void Dados::DFS() {

	std::cout << "\n" << std::endl;

	int combustivel = 0;

	std::vector<std::vector<bool>> visitado(linhas, std::vector<bool>(colunas, false));
	visitado[partida.first][partida.second] = true; // Marca o vértice inicial como visitado

	std::stack<int> portos;
	for (int i = 9; i >= 1; i--) {
		portos.push(i);
	}

	while (!portos.empty()) {

		int atual = portos.top();
		portos.pop();

		for (auto& v : coordenadas) {

			int vizinho = grafo[v.first][v.second] - '0';

			if (!visitado[v.first][v.second] && vizinho == atual) {

				visitado[v.first][v.second] = true;
				std::cout << "Encontrei posto " << atual << " na coordenada (" << v.first << "," << v.second << ")" << std::endl;

				combustivel += abs(partida.first - v.first) + abs(partida.second - v.second);
				partida = v; // troca o ponto de partida

				//caso chegue no último porto
				if (atual == 9) {
					std::cout << "\nCombustivel total: " << combustivel << std::endl;
					return;
				}

				portos.push(vizinho);

			}
		}
	}
}

int main() {

	std::cout << "\n\nTabela de resultados para o teste: " << std::endl;
	std::cout << "Sem contar * como obstaculo e sem retornar para casa: 61" << std::endl;
	std::cout << "* Como obstaculo e sem retornar para casa: 71" << std::endl;
	std::cout << "Tudo certo como deveria: 78" << std::endl;

	Dados Dados("teste.txt");
	Dados.Mapa();
	Dados.DFS();

	return 0;
}#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>


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
	partida = { 0, 0 };

	int portoAtual = 1;

	for (int i = 0; i < linhas; i++) {

		std::getline(file, line);
		std::vector<char> col(colunas, ' ');

		for (int j = 0; j < colunas && j < line.size(); j++) {

			col[j] = line[j];

			//std::cout << "Lendo caractere " << line[j] << " na coordenada (" << i << "," << j << ")" << std::endl;

			if (line[j] >= '1' && line[j] <= '9') { // Se o caractere for um número de 1 até 9

				int atual = line[j] - '0';

				if (atual == portoAtual) {
					partida = { i, j };
					portoAtual++;
				}
				coordenadas.emplace_back(i, j);
			}
		}

		grafo[i] = col;
	}
}

void Dados::DFS() {

	std::cout << "\n" << std::endl;

	int combustivel = 0;

	std::vector<std::vector<bool>> visitado(linhas, std::vector<bool>(colunas, false));
	visitado[partida.first][partida.second] = true; // Marca o vértice inicial como visitado

	std::stack<int> portos;
	for (int i = 9; i >= 1; i--) {
		portos.push(i);
	}

	while (!portos.empty()) {

		int atual = portos.top();
		portos.pop();

		for (auto& v : coordenadas) {

			int vizinho = grafo[v.first][v.second] - '0';

			if (!visitado[v.first][v.second] && vizinho == atual) {

				visitado[v.first][v.second] = true;
				std::cout << "Encontrei posto " << atual << " na coordenada (" << v.first << "," << v.second << ")" << std::endl;

				combustivel += abs(partida.first - v.first) + abs(partida.second - v.second);
				partida = v; // troca o ponto de partida

				//caso chegue no último porto
				if (atual == 9) {
					std::cout << "\nCombustivel total: " << combustivel << std::endl;
					return;
				}

				portos.push(vizinho);

			}
		}
	}
}

int main() {

	std::cout << "\n\nTabela de resultados para o teste: " << std::endl;
	std::cout << "Sem contar * como obstaculo e sem retornar para casa: 61" << std::endl;
	std::cout << "* Como obstaculo e sem retornar para casa: 71" << std::endl;
	std::cout << "Tudo certo como deveria: 78" << std::endl;

	Dados Dados("teste.txt");
	Dados.Mapa();
	Dados.DFS();

	return 0;
}
