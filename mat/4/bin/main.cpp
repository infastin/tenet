#include "adj_list.hpp"

#include <ranges>
#include <fstream>
#include <deque>
#include <queue>

std::istream &
operator>>(std::istream &stream, adj_list &graph)
{
	usize n;
	stream >> n;

	for (usize i = 0; i < n; ++i) {
		graph.insert(i);
	}

	for (usize i = 0; i < n; ++i) {
		for (usize j = 0; j < n; ++j) {
			f64 dist;
			stream >> dist;
			if (dist != 0) {
				graph[i].insert(j, dist);
				graph[j].rinsert(i);
			}
		}
	}

	return stream;
}

f64
karp(adj_list &graph, usize begin, usize end)
{
	f64 flow = 0;

	while (1) {
		oct::vector<usize> parents(graph.size(), usize(-1));
		std::queue<usize> q;

		q.push(begin);

		while (!q.empty()) {
			usize v = q.front();
			q.pop();

			usize pushed = 0;
			for (auto &e : graph[v]) {
				usize u = e.first;

				if (parents[u] != usize(-1) || u == begin || e.second.flow() == e.second.dist()) {
					continue;
				}

				parents[u] = v;
				q.push(u);
				++pushed;

				if (u == end) {
					goto out;
				}
			}

			if (pushed == 0) {
				for (auto iter = graph[v].rbegin(); iter != graph[v].rend(); ++iter) {
					usize u = iter->first;

					if (parents[u] != usize(-1) || u == begin || iter->second.flow() == 0) {
						continue;
					}

					parents[u] = v;
					q.push(u);

					if (u == end) {
						goto out;
					}
				}
			}
		}

		break;

	out:
		f64 min = std::numeric_limits<f64>::infinity();

		for (usize cur = end; parents[cur] != usize(-1); cur = parents[cur]) {
			auto &v = graph[parents[cur]][cur];
			if (v.dist() - v.flow() < min) {
				min = v.dist() - v.flow();
			}
		}

		for (usize cur = end; parents[cur] != usize(-1); cur = parents[cur]) {
			auto &v = graph[parents[cur]][cur];
			v.flow() += min;

			auto &r = graph[cur][parents[cur]];
			r.flow() -= min;
		}

		flow += min;
	}

	return flow;
}

int main(int, char *argv[])
{
	std::fstream in(argv[1], std::ios_base::in);
	//std::fstream out(argv[2], std::ios_base::out);

	adj_list graph;
	in >> graph;

	f64 flow = karp(graph, 0, 6);
	
	oct::fmt::println("%f", flow);

	return 0;
}
