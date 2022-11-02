#ifndef PATH_HPP_EQS1DN5F
#define PATH_HPP_EQS1DN5F

#include <cstddef>
#include <initializer_list>
#include <queue>
#include <stdexcept>
#include <vector>

template<typename T>
class AdjacencyMatrix {
	T *matrix;
	size_t m_size;

public:
	AdjacencyMatrix(size_t size)
	{
		this->m_size = size;
		this->matrix = new T[size * size];
	}

	AdjacencyMatrix(std::initializer_list<std::initializer_list<T>> matrix)
	{
		this->m_size = matrix.size();
		this->matrix = new T[m_size * m_size];

		for (size_t i = 0; i < matrix.size(); ++i) {
			if (matrix[i].size != this->m_size) {
				delete[] this->matrix;
				throw std::logic_error("A non-square matrix is provided");
			}

			for (size_t j = 0; j < matrix[i].size(); ++j) {
				this->matrix[i * this->m_size + j] = std::move(matrix[i][j]);
			}
		}
	}

	~AdjacencyMatrix()
	{
		delete[] matrix;
	}

	T &operator[](size_t i, size_t j)
	{
		return matrix[i * m_size + j];
	}

	constexpr T &operator[](size_t i, size_t j) const
	{
		return matrix[i * m_size + j];
	}

	size_t size() const
	{
		return m_size;
	}
};

std::vector<double> dijkstra(AdjacencyMatrix<double> adj_matr);

#endif /* end of include guard: PATH_HPP_EQS1DN5F */
