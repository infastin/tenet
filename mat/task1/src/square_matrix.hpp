#ifndef SQUARE_MATRIX_HPP_NYKZG57V
#define SQUARE_MATRIX_HPP_NYKZG57V

#include "fmt/core.h"

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <vector>

template<typename T>
class SquareMatrix {
public:
	SquareMatrix(size_t size)
	{
		this->m_size = size;
		this->matrix = new T[size * size];
	}

	SquareMatrix(std::initializer_list<std::initializer_list<T>> matrix)
	{
		this->m_size = matrix.size();
		this->matrix = new T[m_size * m_size];

		for (auto i = 0, row = matrix.begin(); row != matrix.end(); ++row, ++i) {
			if (row->size() != this->m_size) {
				delete[] this->matrix;
				throw std::logic_error("A non-square matrix is provided");
			}

			for (auto j = 0, col = row->begin(); col != row->end(); ++col, ++j) {
				this->matrix[i * this->m_size + j] = std::move(*col);
			}
		}
	}

	~SquareMatrix()
	{
		free(matrix);
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

private:
	T *matrix;
	size_t m_size;
};

#endif /* end of include guard: SQUARE_MATRIX_HPP_NYKZG57V */
