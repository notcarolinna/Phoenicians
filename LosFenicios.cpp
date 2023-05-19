#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <climits>
#include <cmath>

class Dados {
private:
	int linhas;
	int colunas;
	int total_distancia;
	std::string arquivo;

	std::vector<std::vector<char>> mapa;
	std::vector<std::pair<int, int>> postos;
	std::stack<int> pilha;
	std::pair<int, int> partida;

public:
	Dados(std::string arquivo) : arquivo(arquivo) {}
	void Mapa();
	void DFS();
};

void Dados::Mapa() {

	std::ifstream file(arquivo);
	if (!file.is_open()) {
		std::cerr << "Falha ao abrir o arquivo " << arquivo << std::endl;
		return;
	}

	std::string line;
	std::getline(file, line);
	std::stringstream ss(line);
	ss >> linhas >> colunas;

	std::cout << "\n\nLinhas: " << linhas << "\nColunas: " << colunas << std::endl;

	mapa = std::vector<std::vector<char>>(linhas, std::vector<char>(colunas));
	partida = { 0, 0 };

	int portoAtual = 1;

	for (int i = 0; i < linhas; i++) {

		std::getline(file, line);
		std::vector<char> col(colunas, ' ');

		for (int j = 0; j < colunas && j < line.size(); j++) {
			col[j] = line[j];
			//std::cout << "Lendo caractere " << line[j] << " na coordenada (" << i << "," << j << ")" << std::endl;

			if (line[j] >= '1' && line[j] <= '9') { // Se o caractere for um número de 1 até 9
				int num = line[j] - '0';
				if (num == portoAtual) {
					partida = { i, j };
					portoAtual++;
				}
				postos.emplace_back(i, j);
			}
		}

		mapa[i] = col;
	}
}

void Dados::DFS() {
		
	for (int i = 9; i >= 1; i--) {
		pilha.push(i);
	}

	int portoAtual = 1;

	std::cout << "\n" << std::endl;

	while (!pilha.empty()) {

		int num = pilha.top();
		pilha.pop();

		std::cout << "Buscando o posto " << num << "..." << std::endl;

		for (auto& v : postos) {
			if (mapa[v.first][v.second] == num + '0') { // posto encontrado
				std::cout << "Encontrei posto " << num << " na coordenada (" << v.first << "," << v.second << ")" << std::endl;
				total_distancia += abs(partida.first - v.first) + abs(partida.second - v.second);
				partida = v;

				if (num == 9) { // último posto encontrado, encerrando busca
					std::cout << "\nDistancia total percorrida: " << total_distancia << std::endl;
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

	Dados Dados("teste.txt");
	Dados.Mapa();
	Dados.DFS();

	return 0;
}
