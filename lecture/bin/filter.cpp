#include <ranges>
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

int main(int argc, char *argv[])
{
	std::vector<int> vec;
	auto is_even = [](int number) -> bool { return number % 2 == 0; };
	auto square = [](int number) -> int { return number * number; };

	std::ranges::copy(std::views::iota(0, 10)
		| std::views::filter(is_even)
		| std::views::transform(square),
		std::back_inserter(vec));

	for (auto i : vec) {
		std::cout << i << '\n';
	}

	return 0;
}
