#include "path.hpp"

#include <limits>
#include <queue>

class Vertex {
public:
	Vertex(size_t index, double dist)
	{
		this->m_index = index;
		this->m_dist = dist;
	}

	size_t index() const
	{
		return m_index;
	}

	double dist() const
	{
		return m_dist;
	}

	bool operator>(const Vertex &rhs)
	{
		return m_dist > rhs.m_dist;
	}

private:
	size_t m_index;
	double m_dist;
};

std::vector<double> dijkstra(const SquareMatrix<double> &adj_matr, size_t start)
{
	size_t n = adj_matr.size();

	std::vector<bool> visited(n);
	std::fill(visited.begin(), visited.end(), false);

	std::vector<double> dist(n);
	std::fill(dist.begin(), dist.end(), std::numeric_limits<double>::infinity());
	dist[start] = 0;

	std::priority_queue<Vertex, std::vector<Vertex>, std::greater<>> pqueue;
	pqueue.push(Vertex(start, 0));

	while (!pqueue.empty()) {
		Vertex current = std::move(pqueue.top());
		pqueue.pop();

		size_t i = current.index();

		if (visited[i]) {
			continue;
		}

		visited[i] = true;

		for (size_t j = 0; j < n; ++j) {
			if (adj_matr[i, j] == 0) {
				continue;
			}

			double new_dist = dist[i] + adj_matr[i, j];
			if (new_dist < dist[j]) {
				dist[j] = new_dist;
				pqueue.push(Vertex(j, new_dist));
			}
		}
	}

	return dist;
}

std::pair<std::vector<double>, bool> ford_bellman(const SquareMatrix<double> &adj_matr, size_t start)
{
	size_t n = adj_matr.size();

	std::vector<double> dist(n);
	std::fill(dist.begin(), dist.end(), std::numeric_limits<double>::infinity());
	dist[start] = 0;

	for (size_t k = 0; k < n - 1; ++k) {
		for (size_t i = 0; i < n; ++i) {
			for (size_t j = 0; j < n; ++j) {
				if (adj_matr[i, j] == 0) {
					continue;
				}

				double new_dist = dist[i] + adj_matr[i, j];
				if (new_dist < dist[j]) {
					dist[j] = new_dist;
				}
			}
		}
	}

	for (size_t i = 0; i < n; ++i) {
		for (size_t j = 0; j < n; ++j) {
			if (dist[i] > dist[j] + adj_matr[i, j]) {
				return { dist, false };
			}
		}
	}

	return { dist, true };
}

SquareMatrix<double> floyd_warshall(const SquareMatrix<double> &adj_matr)
{
	size_t n = adj_matr.size();
	SquareMatrix<double> dist(n);

	for (size_t i = 0; i < n; ++i) {
		for (size_t j = 0; j < n; ++j) {
			if (i == j) {
				dist[i, j] = 0;
			} else if (adj_matr[i, j] == 0) {
				dist[i, j] = std::numeric_limits<double>::infinity();
			} else {
				dist[i, j] = adj_matr[i, j];
			}
		}
	}

	for (size_t k = 0; k < n; ++k) {
		for (size_t i = 0; i < n; ++i) {
			for (size_t j = 0; j < n; ++j) {
				double new_dist = dist[i, k] + dist[k, j];
				if (dist[i, j] > new_dist) {
					dist[i, j] = new_dist;
				}
			}
		}
	}

	return dist;
}
