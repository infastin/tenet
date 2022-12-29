#ifndef ADJ_LIST_HPP_5HU1KNFG
#define ADJ_LIST_HPP_5HU1KNFG

#include "oct/types.hpp"
#include "oct/cont/vector.hpp"

#include <map>

using namespace oct::types;

namespace detail {

class _edge {
public:
	using size_type = usize;
	using distance_type = f64;

	_edge(distance_type dist = 0)
		: m_dist(dist)
		, m_flow(0)
	{}

	distance_type
	dist() const noexcept
	{ return m_dist; }

	distance_type &
	flow() noexcept
	{ return m_flow; }

	distance_type
	flow() const noexcept
	{ return m_flow; }

private:
	distance_type m_dist;
	distance_type m_flow;
};

class _vertex {
public:
	using size_type = usize;
	using distance_type = f64;
	using edge = _edge;
	using iterator = std::map<usize, edge>::iterator;
	using const_iterator = std::map<usize, edge>::const_iterator;

	_vertex(size_type index) noexcept
		: m_index(index)
	{}

	size_type
	index() const noexcept
	{ return m_index; }

	edge &
	operator[](usize index)
	{
		if (m_edges.contains(index)) {
			return m_edges.at(index);
		}

		return m_redges.at(index);
	}

	bool
	has(usize index)
	{ return m_edges.contains(index) || m_redges.contains(index); }

	void
	insert(size_type index, distance_type dist = 0)
	{ m_edges.emplace(std::piecewise_construct, std::forward_as_tuple(index), std::forward_as_tuple(dist)); }

	void
	rinsert(size_type index)
	{ m_redges.emplace(std::piecewise_construct, std::forward_as_tuple(index), std::forward_as_tuple(0)); }

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

	iterator
	rbegin() noexcept
	{ return m_redges.begin(); }

	iterator
	rend() noexcept
	{ return m_redges.end(); }

	const_iterator
	rbegin() const noexcept
	{ return cbegin(); }

	const_iterator
	rend() const noexcept
	{ return cend(); }

	const_iterator
	crbegin() const noexcept
	{ return m_redges.cbegin(); }
	
	const_iterator
	crend() const noexcept
	{ return m_redges.cend(); }

private:
	std::map<usize, edge> m_edges;
	std::map<usize, edge> m_redges;
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

	size_type
	size() const noexcept
	{ return m_vertices.size(); }

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
