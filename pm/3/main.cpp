#include <iostream>
#include <regex>
#include <string>
#include <vector>

enum {
	TYPE_NONE,
	TYPE_CONST,
	TYPE_VARIABLE,
	TYPE_FUNCTION,
	TYPE_BRACKET,
	TYPE_OPERATOR,
};

struct TermRegex {
	const std::regex re;
	int group;
	int priority;
	int type;
};

struct Term {
	const std::string val;
	int type;
	int priority;
};

const std::vector<TermRegex> regexes = {
	{
		.re = std::regex("(\\d+)(?:\\.\\d+)?"),
		.group = 1,
		.priority = 1,
		.type = TYPE_CONST,
	},
	{
		.re = std::regex("(\\w+)\\(.*\\)"),
		.group = 1,
		.priority = 3,
		.type = TYPE_FUNCTION,
	},
	{

		.re = std::regex("\\w+"),
		.group = 0,
		.priority = 1,
		.type = TYPE_VARIABLE,
	},
	{
		.re = std::regex("\\(|\\)"),
		.group = 0,
		.priority = 0,
		.type = TYPE_BRACKET,
	},
	{
		.re = std::regex("\\+|-"),
		.group = 0,
		.priority = 2,
		.type = TYPE_OPERATOR,
	},
	{
		.re = std::regex("\\s+"),
		.group = 0,
		.priority = -1,
		.type = TYPE_NONE,
	},
};

int main()
{
	std::vector<Term> terms;
	std::string str = "x+111-abs(10-y)";

	for (auto s_iter = str.cbegin(), s_end = str.cend(); s_iter != s_end;) {
		std::smatch matches;
		auto r_iter = regexes.begin();

		for (; r_iter != regexes.end(); ++r_iter) {
			if (std::regex_search(s_iter, s_end, matches, r_iter->re, std::regex_constants::match_continuous)) {
				const std::string match = matches.str(r_iter->group);
				s_iter += match.length();

				terms.push_back({
					.val = std::move(match),
					.type = r_iter->type,
					.priority = r_iter->priority,
				});

				break;
			}
		}

		if (r_iter == regexes.end()) {
			break;
		}
	}

	for (auto term : terms) {
		std::cout << term.val << '\n';
	}

	return 0;
}
