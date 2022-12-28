#ifndef HUNGARIAN_HPP_RNDWGHXY
#define HUNGARIAN_HPP_RNDWGHXY

#include "oct/cont/vector.hpp"
#include "matrix.hpp"
#include "oct/fmt/format.hpp"

#include <concepts>
#include <limits>

namespace detail {

enum class mark_t : u8 {
	none,
	mark,
	prime
};

template<typename T>
requires std::is_arithmetic_v<T>
void
_hungarian_reduce_rows(matrix<T> &table)
{
	usize ngoals = table.size();

	for (usize i = 0; i < ngoals; ++i) {
		T lowest = table[{i, 0}];

		for (usize j = 1; j < ngoals; ++j) {
			if (table[{i, j}] < lowest) {
				lowest = table[{i, j}];
			}
		}

		if (lowest == 0) {
			continue;
		}

		for (usize j = 0; j < ngoals; ++j) {
			table[{i, j}] -= lowest;
		}
	}
}

template<typename T>
requires std::is_arithmetic_v<T>
void
_hungarian_reduce_cols(matrix<T> &table)
{
	usize ngoals = table.size();

	for (usize j = 0; j < ngoals; ++j) {
		T lowest = table[{0, j}];;

		for (usize i = 1; i < ngoals; ++i) {
			if (table[{i, j}] < lowest) {
				lowest = table[{i, j}];
			}
		}

		if (lowest == 0) {
			continue;
		}

		for (usize i = 0; i < ngoals; ++i) {
			table[{i, j}] -= lowest;
		}
	}
}

template<typename T>
requires std::is_arithmetic_v<T>
void
_hungarian_mark(const matrix<T> &table, matrix<mark_t> &marks)
{
	usize ngoals = table.size();

	oct::vector<bool> covered_rows(ngoals, false);
	oct::vector<bool> covered_cols(ngoals, false);

	for (usize i = 0; i < ngoals; ++i) {
		for (usize j = 0; j < ngoals; ++j) {
			if (covered_rows[i] || covered_cols[j]) {
				continue;
			}

			if (table[{i, j}] == 0) {
				marks[{i, j}] = mark_t::mark;
				covered_rows[i] = true;
				covered_cols[j] = true;
			}
		}
	}
}

usize
_hungarian_count_covered(
	const matrix<mark_t> &marks,
	const oct::vector<bool> &covered_rows,
	const oct::vector<bool> &covered_cols
);

void
_hungarian_cover_cols(const matrix<mark_t> &marks, oct::vector<bool> &covered_cols);

template<typename T>
requires std::is_arithmetic_v<T>
std::tuple<usize, usize, bool>
_hungarian_find_prime(
	const matrix<T> &table,
	matrix<mark_t> &marks,
	oct::vector<bool> &covered_rows,
	oct::vector<bool> &covered_cols
) {
	usize ngoals = table.size();

	usize prime_row;
	usize prime_col;

	for (usize i = 0; i < ngoals; ++i) {
		if (covered_rows[i]) {
			continue;
		}

		usize mark_col = usize(-1);

		for (usize j = 0; j < ngoals; ++j) {
			if (marks[{i, j}] == mark_t::mark) {
				mark_col = j;
				continue;
			}

			if (covered_cols[j] || table[{i, j}] != 0) {
				continue;
			}

			marks[{i, j}] = mark_t::prime;
			prime_row = i;
			prime_col = j;

			if (mark_col == usize(-1)) {
				for (usize k = 0; k < ngoals; ++k) {
					if (marks[{i, k}] == mark_t::mark) {
						mark_col = k;
						break;
					}
				}
			}

			if (mark_col != usize(-1)) {
				covered_rows[i] = true;
				covered_cols[mark_col] = false;
				break;
			}

			return std::make_tuple(prime_row, prime_col, true);
		}
	}

	return std::make_tuple(prime_row, prime_col, false);
}

void
_hungarian_alt_marks(
	matrix<mark_t> &marks,
	usize prime_row,
	usize prime_col
);

template<typename T>
requires std::is_arithmetic_v<T>
void
_hungarian_add_and_subtract(
	matrix<T> &table,
	const oct::vector<bool> &covered_rows,
	const oct::vector<bool> &covered_cols
) {
	usize ngoals = table.size();
	T lowest = std::numeric_limits<T>::max();

	for (usize i = 0; i < ngoals; ++i) {
		if (covered_rows[i]) {
			continue;
		}

		for (usize j = 0; j < ngoals; ++j) {
			if (covered_cols[j]) {
				continue;
			}

			if (table[{i, j}] < lowest) {
				lowest = table[{i, j}];
			}
		}
	}

	for (usize i = 0; i < ngoals; ++i) {
		for (usize j = 0; j < ngoals; ++j) {
			if (!covered_rows[i] && !covered_cols[j]) {
				table[{i, j}] -= lowest;
			} else if (covered_rows[i] && covered_cols[j]) {
				table[{i, j}] += lowest;
			}
		}
	}
}


template<typename T>
requires std::is_arithmetic_v<T>
void
_hungarian_print_table(
	const matrix<T> &table
) {

	usize ngoals = table.size();

	for (usize i = 0; i < ngoals; ++i) {
		for (usize j = 0; j < ngoals; ++j) {
			oct::fmt::print("%4v ", table[{i, j}]);
		}

		oct::fmt::print("\n");
	}
}

template<typename T>
requires std::is_arithmetic_v<T>
void
_hungarian_print_table(
	const matrix<T> &table,
	const matrix<mark_t> &marks
) {
	usize ngoals = table.size();

	for (usize i = 0; i < ngoals; ++i) {
		for (usize j = 0; j < ngoals; ++j) {
			if (marks[{i, j}] != mark_t::none) {
				if (marks[{i, j}] == mark_t::mark) {
					oct::fmt::print("%4v*", table[{i, j}]);
				} else if (marks[{i, j}] == mark_t::prime) {
					oct::fmt::print("%4v'", table[{i, j}]);
				}
			} else {
				oct::fmt::print("%4d ", table[{i, j}]);
			}
		}

		oct::fmt::print("\n");
	}
}

template<typename T>
requires std::is_arithmetic_v<T>
void
_hungarian_print_table(
	const matrix<T> &table,
	const matrix<mark_t> &marks,
	const oct::vector<bool> &covered_rows,
	const oct::vector<bool> &covered_cols
) {
	usize ngoals = table.size();

	for (usize i = 0; i < ngoals; ++i) {
		for (usize j = 0; j < ngoals; ++j) {
			if (covered_rows[i] || covered_cols[j]) {
				oct::fmt::print("\e[30;47m");
			}

			if (marks[{i, j}] != mark_t::none) {
				if (marks[{i, j}] == mark_t::mark) {
					oct::fmt::print("%4v*", table[{i, j}]);
				} else if (marks[{i, j}] == mark_t::prime) {
					oct::fmt::print("%4v'", table[{i, j}]);
				}
			} else {
				oct::fmt::print("%4d ", table[{i, j}]);
			}

			if (covered_rows[i] || covered_cols[j]) {
				oct::fmt::print("\e[0m");
			}
		}

		oct::fmt::print("\n");
	}
}

}

template<typename T>
requires std::is_arithmetic_v<T>
oct::vector<usize>
hungarian_assignment(const matrix<T> &distances)
{
	usize ngoals = distances.size();

	oct::vector<bool> covered_rows(ngoals, false);
	oct::vector<bool> covered_cols(ngoals, false);

	matrix<T> table(distances);

	detail::_hungarian_reduce_rows(table);
	detail::_hungarian_reduce_cols(table);

	matrix<detail::mark_t> marks(ngoals, detail::mark_t::none);

	detail::_hungarian_mark(table, marks);

	while (1) {
		detail::_hungarian_cover_cols(marks, covered_cols);

		usize n_covered = detail::_hungarian_count_covered(marks, covered_rows, covered_cols);
		if (n_covered == ngoals) {
			break;
		}

		usize prime_row;
		usize prime_col;

		while (1) {
			auto [pr, pc, found_prime] =
				detail::_hungarian_find_prime(table, marks, covered_rows, covered_cols);

			prime_row = pr;
			prime_col = pc;

			if (found_prime) {
				break;
			}

			detail::_hungarian_add_and_subtract(table, covered_rows, covered_cols);
		}

		detail::_hungarian_alt_marks(marks, prime_row, prime_col);

		for (usize i = 0; i < ngoals; ++i) {
			covered_rows[i] = false;
			covered_cols[i] = false;
		}
	}

	oct::vector<usize> result(ngoals, usize(-1));
	for (usize i = 0; i < ngoals; ++i) {
		for (usize j = 0; j < ngoals; ++j) {
			if (marks[{i, j}] == detail::mark_t::mark) {
				result[i] = j;
			}
		}
	}

	return result;
}

#endif /* end of include guard: HUNGARIAN_HPP_RNDWGHXY */
