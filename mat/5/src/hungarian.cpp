#include "hungarian.hpp"

namespace detail {

usize
_hungarian_count_covered(
	const matrix<mark_t> &marks,
	const oct::vector<bool> &covered_rows,
	const oct::vector<bool> &covered_cols
) {
	usize count = 0;
	usize ngoals = marks.size();

	for (usize i = 0; i < ngoals; ++i) {
		for (usize j = 0; j < ngoals; ++j) {
			if ((covered_rows[i] || covered_cols[j]) && (marks[{i, j}] == mark_t::mark)) {
				++count;
			}
		}
	}

	return count;
}

void
_hungarian_cover_cols(const matrix<mark_t> &marks, oct::vector<bool> &covered_cols)
{
	usize ngoals = marks.size();

	for (usize i = 0; i < ngoals; ++i) {
		for (usize j = 0; j < ngoals; ++j) {
			if (marks[{i, j}] == mark_t::mark) {
				covered_cols[j] = true;
			}
		}
	}
}


void
_hungarian_alt_marks(
	matrix<mark_t> &marks,
	usize prime_row,
	usize prime_col
) {
	usize ngoals = marks.size();

	while (1) {
		usize mark_col = prime_col;
		usize mark_row = std::numeric_limits<usize>::max();

		for (usize i = 0; i < ngoals; ++i) {
			if (i == prime_row) {
				continue;
			}

			if (marks[{i, prime_col}] == mark_t::mark) {
				mark_row = i;
				break;
			}
		}

		marks[{prime_row, prime_col}] = mark_t::mark;

		if (mark_row == std::numeric_limits<usize>::max()) {
			break;
		}

		marks[{mark_row, mark_col}] = mark_t::none;

		for (usize j = 0; j < ngoals; ++j) {
			if (j == prime_col) {
				continue;
			}

			if (marks[{mark_row, j}] == mark_t::prime) {
				prime_row = mark_row;
				prime_col = j;
				break;
			}
		}
	}
}

}
