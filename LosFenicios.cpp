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
}

void Dados::DFS() {

	std::stack<int> portos; // pilha com os portos 
	for (int i = 9; i >= 1; i--) {
		portos.push(i);
	}

	int portoAtual = 1;
	int total_distancia = 0;

	std::cout << "\n" << std::endl;

	while (!portos.empty()) {

		int num = portos.top();
		portos.pop();

		std::cout << "Buscando o posto " << num << "..." << std::endl;

		for (auto& v : coordenadas) {

			if (grafo[v.first][v.second] == num + '0') { // posto encontrado
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
