#include "adj_list.hpp"

#include <ranges>
#include <fstream>
#include <deque>

std::istream &
operator>>(std::istream &stream, adj_list &graph)
{
	usize n;
	stream >> n;

	for (usize i = 0; i < n; ++i) {
		graph.insert(i);

		for (usize j = 0; j < n; ++j) {
			f64 dist;
			stream >> dist;
			if (dist != 0) {
				graph[i].insert(j, dist);
			}
		} 
	}

	return stream;
}

namespace detail {
	template<
		std::ranges::input_range R1,
		std::ranges::input_range R2,
		class Proj1 = std::identity,
		class Proj2 = std::identity,
		std::indirect_strict_weak_order<
			std::projected<std::ranges::iterator_t<R1>, Proj1>,
			std::projected<std::ranges::iterator_t<R2>, Proj2>
		> Comp = std::ranges::less
	>
	bool
	_has_intersection(
		R1 &&first,
		R2 &&second,
		Comp comp = {},
		Proj1 proj1 = {},
		Proj2 proj2 = {}
	) {
		auto begin1 = std::ranges::begin(first);
		auto end1 = std::ranges::end(first);

		auto begin2 = std::ranges::begin(second);
		auto end2 = std::ranges::end(second);

		while (begin1 != end1 && begin2 != end2) {
			if (!comp(proj1(*begin1), proj2(*begin2))) {
				if (!comp(proj2(*begin2), proj1(*begin1))) {
					return true;
				}

				++begin2;
			} else {
				++begin1;
			}
		}

		return false;
	}

	bool
	_bron_contains_unconnected(
		const adj_list &graph,
		const oct::vector<usize> &first,
		const oct::vector<usize> &second
	) {
		for (auto i = first.begin(); i != first.end(); ++i) {
			bool hs = _has_intersection(graph[*i], second, {}, [](const adj_list::edge &e) {
				return e.index();
			});

			if (!hs) {
				return true;
			}
		}

		return false;
	}

	oct::vector<usize>
	_bron_remove_connected(
		const adj_list &graph,
		const oct::vector<usize> &array,
		usize vertex
	) {
		oct::vector<usize> result;

		auto begin1 = array.begin();
		auto end1 = array.end();

		auto begin2 = graph[vertex].begin();
		auto end2 = graph[vertex].end();

		while (begin1 != end1 && begin2 != end2) {
			if (*begin1 < begin2->index()) {
				if (*begin1 != vertex) {
					result.push_back(*begin1);
				}
				++begin1;
			} else {
				if (*begin1 == begin2->index()) {
					++begin1;
				}

				++begin2;
			}
		}

		for (; begin1 != end1; ++begin1) {
			if (*begin1 != vertex) {
				result.push_back(*begin1);
			}
		}

		return result;
	}

	std::pair<usize, usize>
	_bron_extend(
		std::ostream &stream,
		const adj_list &graph,
		oct::vector<usize> &complete,
		oct::vector<usize> &candidates,
		oct::vector<usize> &unwanted
	) {
		usize count = 0;
		usize max = 0;

		while (!candidates.empty() && !_bron_contains_unconnected(graph, unwanted, candidates)) {
			usize vertex = candidates.front();
			complete.push_back(vertex);

			auto new_candidates = _bron_remove_connected(graph, candidates, vertex);
			auto new_unwanted = _bron_remove_connected(graph, unwanted, vertex);

			if (new_candidates.empty() && new_unwanted.empty()) {
				if (max < complete.size()) {
					max = complete.size();
				}

				++count;

				for (auto v : complete) {
					oct::fmt::oprint(stream, "%d ", v);
				}
				oct::fmt::oprint(stream, "\n");
			} else {
				auto [cnt, max_size] = _bron_extend(stream, graph, complete, new_candidates, new_unwanted);
				
				if (max < max_size) {
					max = max_size;
				}

				count += cnt;
			}

			auto vcomp = std::ranges::find(complete, vertex);
			complete.erase(vcomp);

			auto vcan = std::ranges::find(candidates, vertex);
			candidates.erase(vcan);

			unwanted.push_back(vertex);
		}

		return std::make_pair(count, max);
	}
}

std::pair<usize, usize>
bron(std::ostream &stream, const adj_list &graph)
{
	oct::vector<usize> complete;
	oct::vector<usize> candidates;
	oct::vector<usize> unwanted;

	if (graph.empty()) {
		return std::make_pair(0, 0);
	}

	for (auto v : graph) {
		candidates.push_back(v.index());
	}

	return detail::_bron_extend(stream, graph, complete, candidates, unwanted);
}

int main(int, char *argv[])
{
	std::fstream in(argv[1], std::ios_base::in);
	std::fstream out(argv[2], std::ios_base::out);

	adj_list graph;
	in >> graph;

	auto [count, max] = bron(out, graph);
	oct::fmt::println("%d %d", max, count);

	return 0;
}
