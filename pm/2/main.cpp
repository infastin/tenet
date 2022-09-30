#include "t2func.hpp"

#include <iostream>

int main(int argc, char *argv[])
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
	return 0;
}
