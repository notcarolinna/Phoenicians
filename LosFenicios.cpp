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
	bool CaminhoValido(int porto);
	void DFS(std::vector<int>& portosDesobstruidos);
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

			if (line[j] >= '1' && line[j] <= '9') { // Se o caractere for um número de 1 até 9
				int num = line[j] - '0';
				if (num == portoAtual) {
					partida = { i, j };
					portoAtual++;
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
}

bool Dados::CaminhoValido(int porto) {

	std::vector<std::vector<bool>> visitado(linhas, std::vector<bool>(colunas, false));
	visitado[partida.first][partida.second] = true; // Marca o vértice inicial como visitado

	std::stack<std::pair<int, int>> pilha;
	pilha.push(partida);

	while (!pilha.empty()) {
		auto atual = pilha.top();
		pilha.pop();

		int i = atual.first;
		int j = atual.second;

		if (grafo[i][j] - '0' == porto) {
			return true; // Encontrou o porto
		}

		// Verifica os vizinhos (Norte, Sul, Leste, Oeste)
		std::vector<std::pair<int, int>> vizinhos = { {i - 1, j}, {i + 1, j}, {i, j + 1}, {i, j - 1} };

		for (const auto& vizinho : vizinhos) {
			int linha = vizinho.first;
			int coluna = vizinho.second;

			if (linha >= 0 && linha < linhas && coluna >= 0 && coluna < colunas && !visitado[linha][coluna] && grafo[linha][coluna] != '*') {
				visitado[linha][coluna] = true;
				pilha.push(vizinho);
			}
		}
	}

	return false; // Não encontrou o porto
}

void Dados::DFS(std::vector<int>& portosDesobstruidos) {
	std::cout << "\n" << std::endl;

	int combustivel = 0;

	std::vector<std::vector<bool>> visitado(linhas, std::vector<bool>(colunas, false));
	visitado[partida.first][partida.second] = true; // Marca o vértice inicial como visitado

	std::stack<int> portos;

	for (int i = portosDesobstruidos.size() - 1; i >= 0; i--) {
		portos.push(portosDesobstruidos[i]);
	}

	int portoAtual = 1;

	while (!portos.empty()) {
		int atual = portos.top();
		portos.pop();

		std::cout << "\nBuscando o posto " << atual << "..." << std::endl;

		for (auto& v : coordenadas) {
			if (grafo[v.first][v.second] == atual + '0' && CaminhoValido(atual)) {
				std::cout << "Encontrei posto " << atual << " na coordenada (" << v.first << "," << v.second << ")" << std::endl;

				int distancia = abs(partida.first - v.first) + abs(partida.second - v.second);
				std::cout << "Partida: " << partida.first << "," << partida.second << std::endl;
				std::cout << "Chegada: " << v.first << "," << v.second << std::endl;
				std::cout << "Distancia: " << distancia << std::endl;

				combustivel += distancia;
				partida = v;

				if (atual == 9) { // último posto encontrado, encerrando busca
					std::cout << "\nDistancia total percorrida: " << combustivel << std::endl;
					return;
				}

				portoAtual++;
				break;
			}
		}
	}
}

int main() {

	std::cout << "\n\nTabela de resultados para o teste: " << std::endl;
	std::cout << "Sem contar * como obstaculo e sem retornar para casa: 61" << std::endl;
	std::cout << "* Como obstaculo e sem retornar para casa: 71" << std::endl;
	std::cout << "Tudo certo como deveria: 78" << std::endl;

	Dados dados("teste.txt");
	dados.Mapa();

	std::cout << "\n\n" << std::endl;

	std::vector<int> portosDesobstruidos;
	for (int i = 1; i <= 9; i++) {
		if (!dados.CaminhoValido(i)) {
			std::cout << "Porto " << i << " obstruido" << std::endl;
		}
		else {
			portosDesobstruidos.push_back(i);
		}
	}

	dados.DFS(portosDesobstruidos);

	return 0;
}
