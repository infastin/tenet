#ifndef PATH_HPP_EQS1DN5F
#define PATH_HPP_EQS1DN5F

#include "square_matrix.hpp"

std::vector<double> dijkstra(const SquareMatrix<double> &adj_matr, size_t start);
std::pair<std::vector<double>, bool> ford_bellman(const SquareMatrix<double> &adj_matr, size_t start);
SquareMatrix<double> floyd_warshall(const SquareMatrix<double> &adj_matr);

#endif /* end of include guard: PATH_HPP_EQS1DN5F */
