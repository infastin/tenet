#ifndef STACK_UTILS_HPP_AFOKF7MW
#define STACK_UTILS_HPP_AFOKF7MW

#include "strutils.hpp"

#include <array>
#include <memory.h>
#include <string>
#include <vector>

namespace stack {

template<typename T>
void fill_random_const(T &s, size_t len)
{
	const std::string alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	for (size_t i = 0; i < len; ++i) {
		const std::string random = random_string(alphabet, 1000);
		s.push(random);
	}
}

template<typename T>
void fill_random_temp(T &s, size_t len)
{
	const std::string alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	for (size_t i = 0; i < len; ++i) {
		s.push(random_string(alphabet, 1000));
	}
}

template<typename T, typename S, typename A>
bool equal(const T &s1, const S &s2)
{
	T copy_s1 = s1;
	S copy_s2 = s2;

	while (!copy_s1.empty()) {
		const A &v1 = copy_s1.top();
		const A &v2 = copy_s2.top();

		if (v1 != v2)
			return false;

		copy_s1.pop();
		copy_s2.pop();
	}

	if (!copy_s2.empty())
		return false;

	return true;
}

template<typename T, typename A>
void remove_all(T &s, const A &value)
{
	std::vector<A> vec;

	while (!s.empty()) {
		A &val = s.top();

		if (val != value)
			vec.push_back(std::move(val));

		s.pop();
	}

	for (auto iter = vec.rbegin(); iter != vec.rend(); ++iter) {
		s.push(std::move(*iter));
	}
}

template<typename T, typename S, typename A>
bool is_substack(const T &haystack, const S &needle)
{
	if (needle.size() == 0)
		return true;

	if (haystack.size() == 0)
		return false;

	T copy_haystack = haystack;
	S copy_needle = needle;

	A *harr = new A[haystack.size()];
	A *narr = new A[needle.size()];

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

	bool result = memmem(harr, haystack.size(), narr, needle.size()) != NULL;

	delete[] harr;
	delete[] narr;

	return result;
}

}  // namespace stack
#endif /* end of include guard: STACK_UTILS_HPP_AFOKF7MW */
