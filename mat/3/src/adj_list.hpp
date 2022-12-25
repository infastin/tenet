#ifndef ADJ_LIST_HPP_5HU1KNFG
#define ADJ_LIST_HPP_5HU1KNFG

#include "oct/types.hpp"
#include "oct/cont/vector.hpp"

using namespace oct::types;

namespace detail {

class _edge {
public:
	using size_type = usize;
	using distance_type = f64;

	_edge(size_type index, distance_type dist = 0)
		: m_index(index)
		, m_dist(dist)
	{}

	size_type
	index() const noexcept
	{ return m_index; }

	distance_type
	dist() const noexcept
	{ return m_dist; }

private:
	size_type m_index;
	distance_type m_dist;
};

class _vertex {
public:
	using size_type = usize;
	using distance_type = f64;
	using edge = _edge;
	using iterator = oct::vector<edge>::iterator;
	using const_iterator = oct::vector<edge>::const_iterator;

	_vertex(size_type index) noexcept
		: m_index(index)
	{}

	size_type
	index() const noexcept
	{ return m_index; }

	edge &
	operator[](size_type index)
	{ return m_edges[index]; }

	void
	insert(size_type index, distance_type dist = 0)
	{ m_edges.push_back(edge(index, dist)); }

	void
	insert(std::initializer_list<std::pair<size_type, distance_type>> ilist)
	{
		for (auto iter = ilist.begin(); iter != ilist.end(); ++iter) {
			m_edges.push_back(edge(iter->first, iter->second));
		}
	}

	void
	insert(std::initializer_list<size_type> ilist)
	{
		for (auto iter = ilist.begin(); iter != ilist.end(); ++iter) {
			m_edges.push_back(edge(*iter));
		}
	}

	iterator
	begin() noexcept
	{ return m_edges.begin(); }

	iterator
	end() noexcept
	{ return m_edges.end(); }

	const_iterator
	begin() const noexcept
	{ return cbegin(); }

	const_iterator
	end() const noexcept
	{ return cend(); }

	const_iterator
	cbegin() const noexcept
	{ return m_edges.cbegin(); }
	
	const_iterator
	cend() const noexcept
	{ return m_edges.cend(); }

private:
	oct::vector<edge> m_edges;
	size_type m_index;
};

}

class adj_list {
public:
	using size_type = usize;
	using vertex = detail::_vertex;
	using edge = detail::_edge;
	using iterator = oct::vector<vertex>::iterator;
	using const_iterator = oct::vector<vertex>::const_iterator;

	adj_list() noexcept {}

	adj_list(size_type size)
	{
		for (size_type i = 0; i < size; ++i) {
			m_vertices.push_back(i);
		}
	}
	
	bool
	empty() const noexcept
	{ return m_vertices.empty(); }

	void
	insert(size_type index)
	{ m_vertices.push_back(index); }

	vertex &
	operator[](size_type index)
	{ return m_vertices[index]; }

	const vertex &
	operator[](size_type index) const
	{ return m_vertices[index]; }

	iterator
	begin() noexcept
	{ return m_vertices.begin(); }

	iterator
	end() noexcept
	{ return m_vertices.end(); }

	const_iterator
	begin() const noexcept
	{ return cbegin(); }

	const_iterator
	end() const noexcept
	{ return cend(); }

	const_iterator
	cbegin() const noexcept
	{ return m_vertices.cbegin(); }
	
	const_iterator
	cend() const noexcept
	{ return m_vertices.cend(); }

private:
	oct::vector<vertex> m_vertices;
};

#endif /* end of include guard: ADJ_LIST_HPP_5HU1KNFG */
