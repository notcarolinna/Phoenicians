#pragma once
#include <iostream>
#include <vector>
#include <queue>

class Reader {
private:
	std::string arquivo;
	int linhas;
	int colunas;
	std::vector<std::vector<char>> mapa;
	std::pair<int, int> start;
	std::vector<std::pair<int, int>> targets;

public:
	Reader(const std::string& filename);
	bool is_valid(int i, int j);
	void read();
	int bfs();
	int distancia();
};
