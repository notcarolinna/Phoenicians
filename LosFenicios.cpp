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
	void read() {
		std::ifstream file(arquivo);
		std::string line;

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
							}
							targets.push_back(std::make_pair(i, j));
						}
					}
				}
				mapa[i] = row;
			}

			if (firstTarget == 1) {
				std::cerr << "Nao foi encontrado nenhum posto da sequência 1-9." << std::endl;
				return;
			}

			std::cout << "Fui lido com sucesso :)\n" << std::endl;
			file.close();
		}
		else {
			std::cerr << "Nao fui lido com sucesso :(" << std::endl;
		}
	}

	std::vector<std::vector<char>> mapa;
	std::pair<int, int> start;
	std::vector<std::pair<int, int>> targets;
};

int BFS(const std::vector<std::vector<char>>& mapa, const std::pair<int, int>& inicio, const std::pair<int, int> & final) {
	std::vector<std::vector<bool>> percorrido(mapa.size(), std::vector<bool>(mapa[0].size(), false));
	std::vector<std::vector<int>> distancia(mapa.size(), std::vector<int>(mapa[0].size(), 0));

	int linha_movimento[] = { -1, 0, 1, 0 };
	int coluna_movimento[] = { 0, 1, 0, -1 };

	std::queue<std::pair<int, int>> fila;

	fila.push(inicio);
	percorrido[inicio.first][inicio.second] = true;

	while (!fila.empty()) {
		std::pair<int, int> atual = fila.front();
		fila.pop();

		for (int i = 0; i < 4; i++) {
			int nova_linha = atual.first + linha_movimento[i];
			int nova_coluna = atual.second + coluna_movimento[i];

			if (nova_linha >= 0 && nova_linha < mapa.size() &&
				nova_coluna >= 0 && nova_coluna < mapa[0].size() &&
				!percorrido.at(nova_linha).at(nova_coluna) &&
				mapa.at(nova_linha).at(nova_coluna) != '#')
			{
				fila.push(std::make_pair(nova_linha, nova_coluna));
				percorrido.at(nova_linha).at(nova_coluna) = true;
				distancia.at(nova_linha).at(nova_coluna) = distancia.at(atual.first).at(atual.second) + 1;

				if (nova_linha == final.first && nova_coluna == final.second) {
					return distancia.at(nova_linha).at(nova_coluna);
				}
			}
		}
	}
}



int main() {

	Reader reader("teste.txt");
	reader.read();

	std::cout << "Coordenadas do primeiro posto da sequencia: (" << reader.start.first << ", " << reader.start.second << ")" << std::endl;

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
			if (encontrado) {
				break;
			}
		}
		if (encontrado) {
			idx_sequencia++;
			numero_atual = sequencia_desejada[idx_sequencia];
		}
		else {
			std::cerr << "Nao foi possivel encontrar o posto " << numero_atual << "." << std::endl;
			return -1;
		}
	}

}
