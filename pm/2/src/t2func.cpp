#include "t2func.hpp"

namespace task2 {

std::pair<int, int> invalid_brace_seq_idx(std::string str)
{
	std::stack<std::pair<const char &, size_t>> s;

	const std::array<char, 4> open = { '(', '[', '{', '<' };
	const std::array<char, 4> close = { ')', ']', '}', '>' };

	for (size_t i = 0; i < str.length(); ++i) {
		const char &c = str[i];

		if (std::find(open.cbegin(), open.cend(), c) != open.cend()) {
			s.push({ c, i });
			continue;
		}

		auto cb_it = std::find(close.cbegin(), close.cend(), c);
		if (cb_it != close.cend()) {
			if (s.empty())
				return { -1, i };

			const auto &top = s.top();

			size_t brace_idx = close.begin() - cb_it;
			const char &ob = open[brace_idx];

			if (top.first != ob)
				return { top.second, i };

			s.pop();
		}
	}

	return { -1, -1 };
}
}  // namespace task2
