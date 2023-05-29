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

	grafo = std::vector<std::vector<char>>(linhas, std::vector<char>(colunas)); // Criação de uma matriz com o tamanho obtido das linhas e colunas
	partida = { 0, 0 };

	int portoAtual = 1; // Inicializei em 1 pois o percurso começa sempre do primeiro porto 

	for (int i = 0; i < linhas; i++) { // Percorre cada linha
		std::getline(file, line);
		std::vector<char> col(colunas, ' '); // Criação de um vetor com o tamanho do número de colunas

		for (int j = 0; j < colunas && j < line.size(); j++) { // Percorre cada coluna da linha

			col[j] = line[j];

			if (line[j] >= '1' && line[j] <= '9') { // Verifica se o caractere é um número de 1 a 9

				int num = line[j] - '0'; // Converte o caractere para um número, mas pode-se alterar essa verificação para pegar apenas números
				if (num == portoAtual) { // Verifica se o caractere lido é igual ao porto atual
					partida = { i, j }; // Se for, atualiza as coordenadas do porto atual
					portoAtual++;
				}

				coordenadas.emplace_back(i, j); // Adiciona a linha lida na matriz
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

	/*
	Essa função foi um surto depois de tentar muitas vezes fazer meu código reconhecer os asteriscos.
	A lógica teria que ser: verificar se o vizinho é um asterisco e, se for, retorna para o ponto de 
	partida do último porto visitado e procura o próximo caminho sem obstrução. Isso provavelmente 
	envolve recursão.
	*/

	std::vector<std::vector<bool>> visitado(linhas, std::vector<bool>(colunas, false));
	visitado[partida.first][partida.second] = true; // Marca o vértice inicial como visitado

	std::stack<std::pair<int, int>> pilha;
	pilha.push(partida);

	while (!pilha.empty()) {
		auto atual = pilha.top();
		pilha.pop();

		int i = atual.first;
		int j = atual.second;

		if (grafo[i][j] - '0' == porto) { // Verifica se o valor do vértice atual é igual ao porto desejado, se for, o porto foi encontrado
			return true; // Encontrou o porto
		}

		// Verifica os vizinhos (Norte, Sul, Leste, Oeste)
		std::vector<std::pair<int, int>> vizinhos = { {i - 1, j}, {i + 1, j}, {i, j + 1}, {i, j - 1} };

		for (const auto& vizinho : vizinhos) { // Itera sobre caada vizinho na lista de vizinhos
			
			// Obtém as coordenadas do vizinho atual
			int linha = vizinho.first; 
			int coluna = vizinho.second;

			// Verifica se as coordenadas estão dentro dos limites do grafo e se o vizinho não é um asterisco
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

	// Coloca na pilha apenas os portos não obstruídos obtidos a partir da função CaminhoValido e da main
	for (int i = portosDesobstruidos.size() - 1; i >= 0; i--) {
		portos.push(portosDesobstruidos[i]);
	}

	int portoAtual = 1;

	while (!portos.empty()) {
		int atual = portos.top();
		portos.pop();

		std::cout << "\nBuscando o posto " << atual << "..." << std::endl;

		for (auto& v : coordenadas) { // Percorre todas as coordenadas armazenadas no vector coordenadas
			if (grafo[v.first][v.second] == atual + '0') { // Verifica se o caractere correspondente à coordenada atual é igual ao número do porto atual
				std::cout << "Encontrei posto " << atual << " na coordenada (" << v.first << "," << v.second << ")" << std::endl;

				// Cálculo da distância entre a partida e chegada usando a distância de Manhattan
				int distancia = abs(partida.first - v.first) + abs(partida.second - v.second);
				std::cout << "Partida: " << partida.first << "," << partida.second << std::endl;
				std::cout << "Chegada: " << v.first << "," << v.second << std::endl;
				std::cout << "Distancia: " << distancia << std::endl;

				combustivel += distancia;
				partida = v;

				if (atual == 9) {
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
	std::cout << "Contando portos obstruídos mas sem contar o  * no caminhamento: 57" << std::endl;
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
