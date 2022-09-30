#include "strutils.hpp"

#include <random>

std::string random_string(const std::string &alphabet, size_t len)
{
	std::string result;
	result.reserve(len);

	size_t alph_len = alphabet.length();

	if (alph_len == 0)
		return result;

	for (size_t i = 0; i < len; ++i) {
		const char rc = alphabet[rand() % alph_len];
		result.push_back(rc);
	}

	return result;
}
