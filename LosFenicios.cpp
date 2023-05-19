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

	std::vector<std::vector<char>> mapa; // matriz do mapa
	std::pair<int, int> partida;
	std::stack<int> portos; // pilha com os postos
	std::vector<std::pair<int, int>> postos; // coordenadas dos postos

public:
	Dados(std::string arquivo) : arquivo(arquivo) {}
	bool isValid(int linhas, int colunas);
	void Mapa();
	void DFS();
};

bool Dados::isValid(int linhas, int colunas) {

	// verifica se são águas navegáveis ou não (*)

	for (int i = 0; i < linhas; i++) {
		for (int j = 0; j < colunas; j++) {
			if (mapa[i][j] == '*') {
				return true;
			}
		}
	}

	return false;
}

void Dados::Mapa() {
	std::ifstream file(arquivo);
	if (!file.is_open()) {
		std::cerr << "Falha ao abrir o arquivo " << arquivo << std::endl;
		return;
	}

	total_distancia = 0;
	std::string line;
	std::getline(file, line);
	std::stringstream ss(line);
	ss >> linhas >> colunas;
	std::cout << "\n\nLinhas: " << linhas << "\nColunas: " << colunas << std::endl;

	mapa = std::vector<std::vector<char>>(linhas, std::vector<char>(colunas));
	partida = { 0, 0 };

	int portoAtual = 1; // preciso alterar isso, pois o porto 1 não será necessariamente o primeiro

	// recheando minha pilha com portos de 1 a 9

	for (int i = 9; i >= 1; i--) {
		portos.push(i);
	}

	for (int i = 0; i < linhas; i++) {

		std::getline(file, line); 
		std::vector<char> col(colunas, ' ');

		for (int j = 0; j < colunas && j < line.size(); j++) { // itera sobre as colunas

			col[j] = line[j]; // o j lido da line[j] é atribuido ao elemento correspondente em col

			//std::cout << "Lendo caractere " << line[j] << " na coordenada (" << i << "," << j << ")" << std::endl;

			if (line[j] >= '1' && line[j] <= '9') { // verifica se o porto foi encontrado no mapa
				partida = { i, j };
				portoAtual++;
				postos.emplace_back(i, j);
			}
		}

		mapa[i] = col;
	}
}

void Dados::DFS() {
	std::cout << "\nEstou na DFS" << std::endl;

	int portoAtual = 1; // preciso alterar isso, pois o porto 1 não será necessariamente o primeiro

	while (!portos.empty()) {
		int num = portos.top();
		portos.pop();

		// std::cout << "Buscando o posto " << num << "..." << std::endl;

		for (auto& v : postos) {

			if (mapa[v.first][v.second] == num + '0') { // posto encontrado
				if (isValid(v.first, v.second)) { // verifica se o posto é válido
					std::cout << "Encontrei posto " << num << " na coordenada (" << v.first << "," << v.second << ")" << std::endl;
					total_distancia += abs(partida.first - v.first) + abs(partida.second - v.second);
					partida = v;


					// Refazer essa nhaca depois
					if (num == 9) { 
						std::cout << "\nDistancia total percorrida: " << total_distancia << std::endl;
						return;
					}

					portoAtual++;
					break;
				}
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
