#ifndef T2FUNC_HPP_JXGRA4TC
#define T2FUNC_HPP_JXGRA4TC

#include <algorithm>
#include <array>
#include <iostream>
#include <stack>

namespace task2 {

std::pair<int, int> invalid_brace_seq_idx(std::string str);

template<typename Stack>
void sort(Stack &s)
{
	if (s.empty() || s.size() == 1)
		return;

	Stack temp;

	while (!s.empty()) {
		temp.push(std::move(s.top()));
		s.pop();
	}

	while (!temp.empty()) {
		auto value = std::move(temp.top());
		temp.pop();

		while (1) {
			if (s.empty())
				break;

			if (s.top() >= value)
				break;

			temp.push(std::move(s.top()));
			s.pop();
		}

		s.push(std::move(value));
	}
}

namespace details {

template<typename Stack>
void quick_sort(Stack &s, bool reverse)
{
	if (s.empty() || s.size() == 1)
		return;

	Stack lower, higher;

	auto pivot = std::move(s.top());
	s.pop();

	while (!s.empty()) {
		auto value = std::move(s.top());
		s.pop();

		if (value >= pivot)
			higher.push(std::move(value));
		else
			lower.push(std::move(value));
	}

	quick_sort(lower, !reverse);
	quick_sort(higher, !reverse);

	if (reverse) {
		while (!higher.empty()) {
			s.push(std::move(higher.top()));
			higher.pop();
		}

		s.push(std::move(pivot));

		while (!lower.empty()) {
			s.push(std::move(lower.top()));
			lower.pop();
		}
	} else {
		while (!lower.empty()) {
			s.push(std::move(lower.top()));
			lower.pop();
		}

		s.push(std::move(pivot));

		while (!higher.empty()) {
			s.push(std::move(higher.top()));
			higher.pop();
		}
	}
}
}  // namespace details

template<typename Stack>
void quick_sort(Stack &s)
{
	details::quick_sort(s, true);
}

}  // namespace task2

#endif /* end of include guard: T2FUNC_HPP_JXGRA4TC */
