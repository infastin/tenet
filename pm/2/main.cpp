#include "graph.hpp"
#include "t2func.hpp"

#include <iostream>

void test_sort()
{
	std::stack<int> s;

	for (int i = 0; i < 100; ++i) {
		s.push(rand() % 100);
	}

	task2::quick_sort(s);

	while (!s.empty()) {
		std::cout << s.top() << ' ';
		s.pop();
	}

	std::cout << '\n';
}

template<typename T>
void print_graph(const task2::graph<T> &g)
{
	std::cout << "graph {\n";

	for (auto i = g.cbegin(); i < g.cend(); ++i) {
		auto n = *i;
		std::cout << n->data();

		if (n->degree() != 0) {
			std::cout << " -- {";

			for (auto j = n->cbegin(); j < n->cend(); ++j) {
				std::cout << (*j)->data();

				if (static_cast<size_t>((j - n->cbegin()) + 1) != n->degree())
					std::cout << ", ";
			}

			std::cout << "}";
		}

		std::cout << '\n';
	}

	std::cout << "}\n";
}

int main()
{
	task2::graph<int> g({
		{10,  11},
		{ 11, 10},
		{ 11, 12},
		{ 12, 11},
		{ 12, 10},
		{ 10, 12},
	});

	print_graph(g);

	return 0;
}
