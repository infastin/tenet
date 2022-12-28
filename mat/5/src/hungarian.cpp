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

	oct::vector<usize> mark_in_row(ngoals, usize(-1));
	oct::vector<usize> mark_in_col(ngoals, usize(-1));
	oct::vector<usize> prime_in_row(ngoals, usize(-1));

	for (usize i = 0; i < ngoals; ++i) {
		for (usize j = 0; j < ngoals; ++j) {
			if (marks[{i, j}] == mark_t::mark) {
				mark_in_row[i] = j;
				mark_in_col[j] = i;
			} else if (marks[{i, j}] == mark_t::prime) {
				prime_in_row[i] = j;
			}

			if (mark_in_row[i] != usize(-1) && prime_in_row[i] != usize(-1)) {
				break;
			}
		}
	}

	usize row = prime_row;
	usize col = prime_col;

	while (1) {
		marks[{row, col}] = mark_t::mark;
		prime_in_row[row] = usize(-1);

		if (mark_in_row[row] != usize(-1)) {
			break;
		}

		if (mark_in_col[col] == usize(-1)) {
			break;
		}

		usize mark_row = mark_in_col[col];
		usize mark_col = col;

		marks[{mark_row, mark_col}] = mark_t::none;
		mark_in_row[mark_row] = usize(-1);
		mark_in_col[mark_col] = usize(-1);

		row = mark_row;
		col = prime_in_row[row];
	}

	for (usize i = 0; i < ngoals; ++i) {
		if (prime_in_row[i] != usize(-1)) {
			marks[{i, prime_in_row[i]}] = mark_t::none;
		}
	}
}

}
