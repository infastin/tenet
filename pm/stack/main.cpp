#include "stack_arr.hpp"
#include "stack_list.hpp"
#include "stack_utils.hpp"
#include "strutils.hpp"
#include "time.h"

#include <iostream>
#include <stack>

void test_temp()
{
	std::cout << "---- Test Fill ----" << '\n';
	std::cout << "--- Temporary ---\n";

	stack::array<std::string> test_array;
	stack::list<std::string> test_list;
	std::stack<std::string> test_std;

	std::cout << "-- Stack Array --" << '\n';

	clock_t oldc = clock();
	stack::fill_random_temp(test_array, 100000);
	clock_t newc = clock();
	double diff = (double) (newc - oldc) / (double) CLOCKS_PER_SEC;

	std::cout << "Processor time: " << diff << " s.\n";

	std::cout << "-- Stack List --" << '\n';

	oldc = clock();
	stack::fill_random_temp(test_list, 100000);
	newc = clock();
	diff = (double) (newc - oldc) / (double) CLOCKS_PER_SEC;

	std::cout << "Processor time: " << diff << " s.\n";

	std::cout << "-- Stack Std --" << '\n';

	oldc = clock();
	stack::fill_random_temp(test_std, 100000);
	newc = clock();
	diff = (double) (newc - oldc) / (double) CLOCKS_PER_SEC;

	std::cout << "Processor time: " << diff << " s.\n";
}

void test_const()
{
	std::cout << "--- Constant ---\n";

	stack::array<std::string> test_array;
	stack::list<std::string> test_list;
	std::stack<std::string> test_std;

	std::cout << "-- Stack Array --" << '\n';

	const std::string alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	std::string *random_array = new std::string[100000];
	for (size_t i = 0; i < 100000; ++i) {
		random_array[i] = random_string(alphabet, 1000);
	}

	clock_t oldc = clock();
	stack::fill_random_const(test_array, 100000, random_array);
	clock_t newc = clock();
	double diff = (double) (newc - oldc) / (double) CLOCKS_PER_SEC;

	std::cout << "Processor time: " << diff << " s.\n";

	std::cout << "-- Stack List --" << '\n';

	for (size_t i = 0; i < 100000; ++i) {
		random_array[i] = random_string(alphabet, 1000);
	}

	oldc = clock();
	stack::fill_random_const(test_array, 100000, random_array);
	newc = clock();
	diff = (double) (newc - oldc) / (double) CLOCKS_PER_SEC;

	std::cout << "Processor time: " << diff << " s.\n";

	std::cout << "-- Stack Std --" << '\n';

	for (size_t i = 0; i < 100000; ++i) {
		random_array[i] = random_string(alphabet, 1000);
	}

	oldc = clock();
	stack::fill_random_const(test_array, 100000, random_array);
	newc = clock();
	diff = (double) (newc - oldc) / (double) CLOCKS_PER_SEC;

	std::cout << "Processor time: " << diff << " s.\n";

	delete[] random_array;
}

void test_remove()
{
	std::cout << "---- Remove All ----\n";

	stack::array<int> test_array;

	test_array.push(10);
	test_array.push(10);
	test_array.push(20);
	test_array.push(10);
	test_array.push(31);
	test_array.push(14);
	test_array.push(10);
	test_array.push(33);
	test_array.push(10);

	stack::remove_all(test_array, 10);

	while (!test_array.empty()) {
		std::cout << test_array.top() << ' ';
		test_array.pop();
	}

	std::cout << '\n';
}

void test_equal()
{
	std::cout << "---- Equal ----\n";

	stack::array<int> haystack;

	haystack.push(10);
	haystack.push(10);
	haystack.push(20);
	haystack.push(10);
	haystack.push(31);
	haystack.push(14);
	haystack.push(10);
	haystack.push(33);
	haystack.push(10);

	stack::list<int> needle;

	needle.push(10);
	needle.push(10);
	needle.push(20);
	needle.push(10);
	needle.push(31);
	needle.push(14);
	needle.push(10);
	needle.push(33);
	needle.push(10);

	std::cout << stack::equal<stack::array<int>, stack::list<int>, int>(haystack, needle) << '\n';
}

void test_substack()
{
	std::cout << "---- Substack ----\n";

	stack::array<int> haystack;

	haystack.push(10);
	haystack.push(10);
	haystack.push(20);
	haystack.push(10);
	haystack.push(31);
	haystack.push(14);
	haystack.push(10);
	haystack.push(33);
	haystack.push(10);

	stack::list<int> needle;

	needle.push(20);
	needle.push(10);
	needle.push(31);

	std::cout << stack::is_substack<stack::array<int>, stack::list<int>, int>(haystack, needle) << '\n';
}

void test_sort()
{
	std::cout << "---- Sort ----\n";

	stack::array<int> test_array;

	test_array.push(1);
	test_array.push(4);
	test_array.push(0);
	test_array.push(-1);
	test_array.push(30);

	stack::sort(test_array);

	while (!test_array.empty()) {
		std::cout << test_array.top() << ' ';
		test_array.pop();
	}

	std::cout << '\n';
}

int main(int argc, char *argv[])
{
	test_temp();
	test_const();
	test_remove();
	test_substack();
	test_sort();
	test_equal();

	return 0;
}
