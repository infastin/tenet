#ifndef STACK_UTILS_HPP_AFOKF7MW
#define STACK_UTILS_HPP_AFOKF7MW

#include "strutils.hpp"

#include <array>
#include <memory.h>
#include <string>
#include <vector>

namespace task1 {

template<typename Stack>
void fill_random_temp(Stack &s, size_t len)
{
	const std::string alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	for (size_t i = 0; i < len; ++i) {
		const std::string random = random_string(alphabet, 1000);
		s.push(random);
	}
}

template<typename Stack>
void fill_random_const(Stack &s, size_t len, std::string *random_array)
{
	const std::string alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	for (size_t i = 0; i < len; ++i) {
		s.push(std::move(random_array[i]));
	}
}

template<typename Stack1, typename Stack2, typename T>
bool equal(const Stack1 &s1, const Stack2 &s2)
{
	Stack1 copy_s1 = s1;
	Stack2 copy_s2 = s2;

	while (!copy_s1.empty()) {
		const T &v1 = copy_s1.top();
		const T &v2 = copy_s2.top();

		if (v1 != v2)
			return false;

		copy_s1.pop();
		copy_s2.pop();
	}

	if (!copy_s2.empty())
		return false;

	return true;
}

template<typename Stack, typename T>
void remove_all(Stack &s, const T &value)
{
	Stack temp;

	while (!s.empty()) {
		T &val = s.top();

		if (val != value)
			temp.push(std::move(val));

		s.pop();
	}

	while (!temp.empty()) {
		s.push(std::move(temp.top()));
		temp.pop();
	}
}

template<typename Stack1, typename Stack2, typename T>
bool is_substack(const Stack1 &haystack, const Stack2 &needle)
{
	if (needle.size() == 0)
		return true;

	if (haystack.size() == 0)
		return false;

	Stack1 copy_haystack = haystack;
	Stack2 copy_needle = needle;

	T *harr = new T[haystack.size()];
	T *narr = new T[needle.size()];

	for (size_t i = haystack.size() - 1;; --i) {
		harr[i] = std::move(copy_haystack.top());
		copy_haystack.pop();

		if (i == 0)
			break;
	}

	for (size_t i = needle.size() - 1;; --i) {
		narr[i] = std::move(copy_needle.top());
		copy_needle.pop();

		if (i == 0)
			break;
	}

	bool result = memmem(harr, haystack.size() * sizeof(T), narr, needle.size() * sizeof(T)) != NULL;

	delete[] harr;
	delete[] narr;

	return result;
}
}  // namespace task1
#endif /* end of include guard: STACK_UTILS_HPP_AFOKF7MW */
