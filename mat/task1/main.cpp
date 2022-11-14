#include "path.hpp"

#include <fmt/core.h>
#include <fmt/ostream.h>
#include <fstream>
#include <iostream>

SquareMatrix<double> read_matrix(std::istream &stream)
{
	size_t n = 0;
	stream >> n;

	SquareMatrix<double> result(n);

	for (size_t i = 0; i < n; ++i) {
		for (size_t j = 0; j < n; ++j) {
			double number;
			stream >> number;
			result[i, j] = number;
		}
	}

	return result;
}

int main(int argc, char *argv[])
{
	if (argc != 3) {
		fmt::print(std::cerr, "Expected two arguments\n");
		exit(EXIT_FAILURE);
	}

	std::ifstream file;
	file.open(argv[1]);

	size_t start;
	sscanf(argv[2], "%lu", &start);

	SquareMatrix<double> matr = read_matrix(file);
	std::vector<double> dist_dijkstra = dijkstra(matr, start);

	fmt::print("Dijkstra:\n");
	for (auto d : dist_dijkstra) {
		fmt::print("{: <6.2f} ", d);
	}

	auto [dist_ford, has_negative] = ford_bellman(matr, start);

	fmt::print("\nFord Bellman:\n");
	if (has_negative) {
		fmt::print("Negative");
	} else {
		for (auto d : dist_ford) {
			fmt::print("{: <6.2f} ", d);
		}
	}

	SquareMatrix<double> matr_floyd = floyd_warshall(matr);

	fmt::print("\nFloyd Warshall:\n");
	for (size_t i = 0; i < matr.size(); ++i) {
		for (size_t j = 0; j < matr.size(); ++j) {
			fmt::print("{: <8.2f}", matr[i, j]);
		}
		fmt::print("\n");
	}

	return 0;
}
