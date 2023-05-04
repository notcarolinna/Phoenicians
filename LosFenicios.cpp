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
	std::cout << "Estou na function Mapa" << std::endl;

	int total_distancia = 0;
	int p[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int num_posto = 0;
	int numero_atual = p[num_posto];

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
			std::cout << "Lendo caractere " << line[j] << " na coordenada (" << i << "," << j << ")" << std::endl;

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
}

int Dados::Distancia() {

	//Coisitas a serem feitas:
	//Verificar se o primeiro posto está obstruído, caso esteja, começar a partir do próximo
	//Fazer isso usando pilha, para que após chegar no último posto, retornar ao primeiro

	int n = postos.size();
	std::vector<bool> visitado(n, false);
	std::stack<int> caderninho;
	caderninho.push(0);
	visitado[0] = true;
	int atual = 0;
	int distancia = 0;

	std::cout << "\n\nEstou na function Distancia" << std::endl;

	while (!caderninho.empty()) { // enquanto houver postos no caderninho
		int next = -1;

		for (int i = atual + 1; i < n; i++) {
			if (!visitado[i] && mapa[postos[atual].first][postos[atual].second] == mapa[postos[i].first][postos[i].second]) {
				// Verifica se o próximo ponto é um obstáculo
				if (mapa[postos[i].first][postos[i].second] != '*') {
					if (next == -1 || abs(postos[atual].first - postos[i].first) + abs(postos[atual].second - postos[i].second) < abs(postos[atual].first - postos[next].first) + abs(postos[atual].second - postos[next].second)) {
						next = i;
					}
				}
			}
		}

		if (next == -1) {
			if (atual == 0) {
				return -1; // Não foi possível alcançar o primeiro posto
			}
			distancia -= abs(postos[atual].first - postos[atual - 1].first) + abs(postos[atual].second - postos[atual - 1].second);
			atual--;
		}

		else {
			visitado[next] = true;
			caderninho.push(next);
			int dist_postos = abs(postos[atual].first - postos[next].first) + abs(postos[atual].second - postos[next].second);
			distancia += dist_postos;
			std::cout << "Distancia entre posto " << atual << " e posto " << next << ": " << dist_postos << std::endl;
			atual = next;
			if (caderninho.size() == n) {
				return distancia; // Todos os postos foram visitados
			}
		}
	}

	return -1; // Caso não seja possível visitar todos os postos
}

int main() {
	Dados Dados("teste.txt");
	Dados.Mapa();
	Dados.Distancia();

	return 0;
}
