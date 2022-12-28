#ifndef MATRIX_HPP_FPFKHZWJ
#define MATRIX_HPP_FPFKHZWJ

#include "oct/memory.hpp"
#include "oct/types.hpp"

using namespace oct::types;

template<typename T, class Allocator = std::allocator<T>>
class matrix {
private:
	using _alloc_traits = oct::allocator_traits<Allocator>;

public:
	using size_type = usize;
	using value_type = T;
	using reference = value_type &;
	using const_reference = const value_type &;

	matrix(size_type size)
		: m_allocator(Allocator())
		, m_size(size)
	{ m_data = _alloc_traits::create_n(m_allocator, m_size * m_size); }

	matrix(size_type size, const T &value)
		: m_allocator(Allocator())
		, m_size(size)
	{ m_data = _alloc_traits::create_n(m_allocator, m_size * m_size, value); }

	matrix(std::initializer_list<std::initializer_list<T>> ilist)
	{
		for (auto iter = ilist.begin(); iter != ilist.end(); ++iter) {
			if (iter->size() != ilist.size()) {
				throw std::invalid_argument("matrix: invalid initializer list");
			}
		}

		m_size = ilist.size();
		m_data = _alloc_traits::allocate(m_allocator, m_size * m_size);

		size_type i = 0;
		for (auto iter = ilist.begin(); iter != ilist.end(); ++iter, ++i) {
			size_type j = 0;
			for (auto jter = iter->begin(); jter != iter->end(); ++jter, ++j) {
				_alloc_traits::construct(m_allocator, &m_data[i * m_size + j], *jter);
			}
		}
	}

	matrix(const matrix &other)
		: m_allocator(_alloc_traits::select_on_copy(other.m_allocator))
	{ _init_copy(other); }

	~matrix() noexcept
	{
		for (size_type i = 0; i < m_size; ++i) {
			for (size_type j = 0; j < m_size; ++j) {
				_alloc_traits::destroy(m_allocator, &m_data[i * m_size + j]);
			}
		}

		_alloc_traits::deallocate(m_allocator, m_data, m_size * m_size);
	}

	size_type
	size() const noexcept
	{ return m_size; }

	reference
	operator[](std::pair<size_type, size_type> ij)
	{
		if (ij.first >= m_size || ij.second >= m_size) {
			throw std::out_of_range("[]: index is out of bounds");
		}

		return m_data[ij.first * m_size + ij.second];
	}

	const_reference
	operator[](std::pair<size_type, size_type> ij) const
	{
		if (ij.first >= m_size || ij.second >= m_size) {
			throw std::out_of_range("[]: index is out of bounds");
		}

		return m_data[ij.first * m_size + ij.second];
	}

	friend void
	swap(matrix &lhs, matrix &rhs) noexcept
	{ lhs.swap(rhs); }
	
	void
	swap(matrix &other) noexcept
	{ swap(std::move(other)); }

	friend void
	swap(matrix &lhs, matrix &&rhs) noexcept
	{ lhs.swap(std::move(rhs)); }

	void
	swap(matrix &&other) noexcept
	{
		_alloc_traits::on_swap(m_allocator, other.m_allocator);
		std::ranges::swap(m_data, other.m_data);
		std::ranges::swap(m_size, other.m_size);
	}

private:
	Allocator m_allocator;
	T *m_data;
	size_type m_size;

	void
	_init_copy(const matrix &other)
	{
		m_size = other.m_size;
		m_data = _alloc_traits::allocate(m_allocator, m_size * m_size);

		for (size_type i = 0; i < m_size; ++i) {
			for (size_type j = 0; j < m_size; ++j) {
				_alloc_traits::construct(m_allocator, &m_data[i * m_size + j], other.m_data[i * m_size + j]);
			}
		}
	}
};

#endif /* end of include guard: MATRIX_HPP_FPFKHZWJ */
