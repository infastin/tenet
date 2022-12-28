#include "matrix.hpp"
#include "hungarian.hpp"
#include "oct/fmt/format.hpp"

#include <ranges>
#include <fstream>
#include <deque>

int main(int, char *argv[])
{
	matrix<u32> m {
		{ 82, 83, 69, 92 },
		{ 77, 37, 49, 92 },
		{ 11, 69, 5, 86 },
		{ 8, 9, 98, 23 }
	};

	oct::vector<usize> assigned = hungarian_assignment(m);

	for (usize i = 0; i < 4; ++i) {
		oct::fmt::print("%v ", assigned[i]);
	}

	return 0;
}
