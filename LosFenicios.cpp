#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>

class Reader {
public:
	std::string arquivo;
	int linhas;
	int colunas;

	Reader(std::string arquivo) : arquivo(arquivo) {}
	std::vector<std::vector<char>> mapa;
	std::pair<int, int> start;
	std::vector<std::pair<int, int>> targets;
	void read();
};


void Reader::read() {
	std::ifstream file(arquivo);
	std::string line;
	std::vector<std::pair<int, int>> direcoes = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

	if (file.is_open()) {
		getline(file, line);
		std::stringstream(line) >> linhas;
		std::stringstream(line.substr(line.find(' ') + 1)) >> colunas;

		// Verificação para saber se o número de linhas e colunas
		// foram obtidos corretamente
		std::cout << "Linhas: " << linhas << "\nColunas: " << colunas << std::endl;

		mapa = std::vector<std::vector<char>>(linhas, std::vector<char>(colunas));

		start = std::make_pair(0, 0); // primeiro posto da sequência não obstruído
		int firstTarget = 1; // primeiro alvo da sequência não obstruído
		bool foundFirstTarget = false; // se já encontrou o primeiro posto da sequência

		for (int i = 0; i < linhas; i++) {
			getline(file, line);
			std::vector<char> row;
			for (int j = 0; j < colunas; j++) {
				if (j < line.size()) {
					row.push_back(line[j]);

					if (line[j] >= '1' && line[j] <= '9') {
						int num;
						std::stringstream(line.substr(j, 1)) >> num;

						if (num == firstTarget) {
							start = std::make_pair(i, j);
							firstTarget++;
							foundFirstTarget = true;
						}
						targets.push_back(std::make_pair(i, j));
					}
				}
			}
			mapa[i] = row;
		}
	}
	else {
		std::cerr << "Nao foi possivel abrir o arquivo " << arquivo << std::endl;
	}
}


int main() {
	Reader reader("teste.txt");
	reader.read();

	std::cout << "Coordenadas do primeiro posto da sequencia: (" << reader.start.first << ", " << reader.start.second << ")\n" << std::endl;

	int total_distancia = 0;

	int sequencia_desejada[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 1 };
	int idx_sequencia = 0;
	int numero_atual = sequencia_desejada[idx_sequencia];

	while (idx_sequencia < 10) {
		bool encontrado = false;

		for (int i = 0; i < reader.linhas; i++) {
			for (int j = 0; j < reader.colunas; j++) {
				if (reader.mapa[i][j] == '0' + numero_atual) {
					reader.targets.push_back(std::make_pair(i, j));
					std::cout << "Posto " << numero_atual << " encontrado nas coordenadas (" << i << ", " << j << ")" << std::endl;
					encontrado = true;
					break;
				}
			}
		}

		if (encontrado) {
			idx_sequencia++;
			numero_atual = sequencia_desejada[idx_sequencia];
		}
	}

	return 0;
}
