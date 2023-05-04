#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <climits>
#include <cmath>

class Reader {
public:
	std::string arquivo;
	int linhas;
	int colunas;

	Reader(std::string arquivo) : arquivo(arquivo) {}
	std::vector<std::vector<char>> mapa;
	std::pair<int, int> start;
	std::vector<std::pair<int, int>> postos;
	void read();
};

void Reader::read() {
	std::ifstream file(arquivo);
	if (!file.is_open()) {
		std::cerr << "Falha ao abrir o arquivo " << arquivo << std::endl;
		return;
	}

	std::string line;
	std::getline(file, line);
	std::stringstream ss(line);
	ss >> linhas >> colunas;
	std::cout << "Linhas: " << linhas << "\nColunas: " << colunas << std::endl;

	mapa = std::vector<std::vector<char>>(linhas, std::vector<char>(colunas));
	start = { 0, 0 };
	int firstTarget = 1;
	bool foundFirstTarget = false;

	for (int i = 0; i < linhas; i++) {
		std::getline(file, line);
		std::vector<char> col(colunas, ' ');
		for (int j = 0; j < colunas && j < line.size(); j++) {
			col[j] = line[j];
			if (line[j] >= '1' && line[j] <= '9') {
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
}



int main() {
	Reader reader("teste.txt");
	reader.read();

	int total_distancia = 0;
	int postos[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int num_posto = 0;
	int numero_atual = postos[num_posto];

	while (num_posto < 10) {
		bool encontrado = false;

		for (int i = 0; i < reader.linhas; i++) {
			for (int j = 0; j < reader.colunas; j++) {
				if (reader.mapa[i][j] == '0' + numero_atual) {
					reader.postos.push_back(std::make_pair(i, j));
					std::cout << "Posto " << numero_atual << " encontrado nas coordenadas (" << i << ", " << j << ")" << std::endl;
					encontrado = true;
					break;
				}
			}
		}

		if (encontrado) {
			num_posto++;
			numero_atual = postos[num_posto];
		}
	}

	return 0;
}
