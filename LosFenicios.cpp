#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <climits>
#include <cmath>

class Dados {
public:
	std::string arquivo;
	int linhas;
	int colunas;

	Dados(std::string arquivo) : arquivo(arquivo) {}
	std::vector<std::vector<char>> mapa;
	std::pair<int, int> start;
	std::vector<std::pair<int, int>> postos;
	void Mapa();
	int Distancia();
};

void Dados::Mapa() {
	std::ifstream file(arquivo);
	if (!file.is_open()) {
		std::cerr << "Falha ao abrir o arquivo " << arquivo << std::endl;
		return;
	}

	std::cout << "\n\nTabela de resultados para o teste: " << std::endl;
	std::cout << "Sem contar * como obstaculo e sem retornar para casa: 61" << std::endl;
	std::cout << "* Como obstaculo e sem retornar para casa: 71" << std::endl;
	std::cout << "Tudo certo como deveria: 78" << std::endl;

	int total_distancia = 0;
	std::string line;
	std::getline(file, line);
	std::stringstream ss(line);
	ss >> linhas >> colunas;
	std::cout << "\n\nLinhas: " << linhas << "\nColunas: " << colunas << std::endl;

	mapa = std::vector<std::vector<char>>(linhas, std::vector<char>(colunas));
	start = { 0, 0 };
	int firstTarget = 1;
	bool foundFirstTarget = false;

	std::stack<int> pilha;
	for (int i = 9; i >= 1; i--) {
		pilha.push(i);
	}

	for (int i = 0; i < linhas; i++) {

		std::getline(file, line);
		std::vector<char> col(colunas, ' ');

		for (int j = 0; j < colunas && j < line.size(); j++) {
			col[j] = line[j];
			//std::cout << "Lendo caractere " << line[j] << " na coordenada (" << i << "," << j << ")" << std::endl;

			if (line[j] >= '1' && line[j] <= '9') { // Se o caractere for um número de 1 até 9
				int num = line[j] - '0';
				if (num == firstTarget) {
					start = { i, j };
					firstTarget++;
					foundFirstTarget = true;
				}
				postos.emplace_back(i, j);
			}
		}

		mapa[i] = col;
	}

	// Aqui eu começo a fazer a busca por profundidade ------------------------------------------------------------------------------

	std::cout << "\n" << std::endl;

	while (!pilha.empty()) {

		int num = pilha.top();
		pilha.pop();

		std::cout << "Buscando o posto " << num << "..." << std::endl;

		for (auto& v : postos) {
			if (mapa[v.first][v.second] == num + '0') { // posto encontrado
				std::cout << "Encontrei posto " << num << " na coordenada (" << v.first << "," << v.second << ")" << std::endl;
				total_distancia += abs(start.first - v.first) + abs(start.second - v.second);
				start = v;

				if (num == 9) { // último posto encontrado, encerrando busca
					std::cout << "\nDistancia total percorrida: " << total_distancia << std::endl;
					return;
				}

				foundFirstTarget = true;
				firstTarget++;
				break;
			}
		}

		if (!foundFirstTarget) {
			std::cerr << "Não foi possível encontrar o posto " << num << std::endl;
			return;
		}
		foundFirstTarget = false;
	}
}


int main() {
	Dados Dados("teste.txt");
	Dados.Mapa();
	return 0;
}
