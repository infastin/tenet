#include "matrix.hpp"
#include "hungarian.hpp"
#include "oct/fmt/format.hpp"

#include <ranges>
#include <fstream>
#include <deque>

template<typename T>
std::istream &
operator>>(std::istream &stream, matrix<T> &matr)
{
	usize n;
	stream >> n;

	matr = matrix<T>(n);

	for (usize i = 0; i < n; ++i) {
		for (usize j = 0; j < n; ++j) {
			stream >> matr[{i, j}];
		}
	}

	return stream;
}

int main(int, char *argv[])
{
	std::fstream in(argv[1], std::ios_base::in);
	std::fstream out(argv[2], std::ios_base::out);

	matrix<f64> matr;
	in >> matr;

	oct::vector<usize> assigned = hungarian_assignment(matr);
	
	f64 sum = 0;
	for (usize i = 0; i < matr.size(); ++i) {
		sum += matr[{i, assigned[i]}];
		oct::fmt::oprintln(out, "%d: %d", i, assigned[i]);
	}
	
	oct::fmt::println("%f", sum);

	return 0;
}
