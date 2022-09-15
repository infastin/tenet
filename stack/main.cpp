#include "stack_arr.hpp"
#include "stack_list.hpp"
#include "stack_utils.hpp"
#include "strutils.hpp"
#include "time.h"

#include <iostream>
#include <stack>

void test_temp()
{
	std::cout << "Temporary\n";

	stack::array<std::string> test_array;
	stack::list<std::string> test_list;
	std::stack<std::string> test_std;

	std::cout << "Stack Array" << '\n';

	clock_t oldc = clock();
	stack::fill_random_temp(test_array, 100000);
	clock_t newc = clock();
	double diff = (double) (newc - oldc) / (double) CLOCKS_PER_SEC;

	std::cout << "Processor time: " << diff << " s.\n";

	std::cout << "Stack List" << '\n';

	oldc = clock();
	stack::fill_random_temp(test_list, 100000);
	newc = clock();
	diff = (double) (newc - oldc) / (double) CLOCKS_PER_SEC;

	std::cout << "Processor time: " << diff << " s.\n";

	std::cout << "Stack Std" << '\n';

	oldc = clock();
	stack::fill_random_temp(test_std, 100000);
	newc = clock();
	diff = (double) (newc - oldc) / (double) CLOCKS_PER_SEC;

	std::cout << "Processor time: " << diff << " s.\n";
}

void test_const()
{
	std::cout << "Constant\n";

	stack::array<std::string> test_array;
	stack::list<std::string> test_list;
	std::stack<std::string> test_std;

	std::cout << "Stack Array" << '\n';

	clock_t oldc = clock();
	stack::fill_random_const(test_array, 100000);
	clock_t newc = clock();
	double diff = (double) (newc - oldc) / (double) CLOCKS_PER_SEC;

	std::cout << "Processor time: " << diff << " s.\n";

	std::cout << "Stack List" << '\n';

	oldc = clock();
	stack::fill_random_const(test_list, 100000);
	newc = clock();
	diff = (double) (newc - oldc) / (double) CLOCKS_PER_SEC;

	std::cout << "Processor time: " << diff << " s.\n";

	std::cout << "Stack Std" << '\n';

	oldc = clock();
	stack::fill_random_const(test_std, 100000);
	newc = clock();
	diff = (double) (newc - oldc) / (double) CLOCKS_PER_SEC;

	std::cout << "Processor time: " << diff << " s.\n";
}

int main(int argc, char *argv[])
{
	stack::array<int> sa;
	std::stack<int> sstd;

	sa.push(11);
	sa.push(12);
	sa.push(14);

	sstd.push(15);

	std::cout << stack::is_substack<stack::array<int>, std::stack<int>, int>(sa, sstd) << std::endl;

	return 0;
}
