#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <vector>

enum TermType {
	TYPE_NONE = 0,
	TYPE_CONST = 1,
	TYPE_VARIABLE = 2,
	TYPE_FUNCTION = 4,
	TYPE_BINARY_OPERATOR = 8,
	TYPE_UNARY_OPERATOR = 16,
	TYPE_OPEN_BRACKET = 32,
	TYPE_CLOSING_BRACKET = 64,
	TYPE_BRACKETS = TYPE_OPEN_BRACKET | TYPE_CLOSING_BRACKET,
	TYPE_EXPR = TYPE_CONST | TYPE_VARIABLE | TYPE_FUNCTION | TYPE_BRACKETS,
	TYPE_OPERATOR = TYPE_BINARY_OPERATOR | TYPE_UNARY_OPERATOR,
	TYPE_ANY = TYPE_EXPR | TYPE_OPERATOR,
};

struct Term {
	std::string val;
	TermType type;
	int priority;
};

struct TermRegex {
	std::regex re;
	std::vector<Term> terms;

	int recursive_group;
	TermType type;
	int group;
	int priority;

	TermRegex(const std::string &re_str, TermType type, int priority = -1, int group = 0, int recursive_group = -1)
	{
		this->re = std::regex(re_str);
		this->type = type;
		this->priority = priority;
		this->group = group;
		this->recursive_group = recursive_group;
	}
};

const std::vector<TermRegex> regexes = {
	TermRegex("(\\d+(?:\\.\\d+)?)", TYPE_CONST, 1, 1),
	TermRegex("(\\w+)(\\(.*\\))", TYPE_FUNCTION, 4, 1, 2),
	TermRegex("\\w+", TYPE_VARIABLE, 1),
	TermRegex("\\s+", TYPE_NONE),
	TermRegex("\\(", TYPE_OPEN_BRACKET, 0),
	TermRegex("\\)", TYPE_CLOSING_BRACKET, 0),
	TermRegex("\\+|-", TYPE_UNARY_OPERATOR, 4),
	TermRegex("\\+|-", TYPE_BINARY_OPERATOR, 1),
};

std::vector<Term> parse(const std::string &str)
{
	std::vector<Term> terms;

	int prev_type = TYPE_NONE;
	int next_type = TYPE_UNARY_OPERATOR | TYPE_EXPR;

	for (auto s_iter = str.cbegin(), s_end = str.cend(); s_iter != s_end;) {
		std::smatch matches;

		auto r_iter = regexes.begin();
		for (; r_iter != regexes.end(); ++r_iter) {
			if (std::regex_search(s_iter, s_end, matches, r_iter->re, std::regex_constants::match_continuous)) {
				if (r_iter->type == TYPE_UNARY_OPERATOR && (prev_type & TYPE_EXPR)) {
					continue;
				} else if (r_iter->type == TYPE_BINARY_OPERATOR && !(prev_type & TYPE_EXPR)) {
					continue;
				} else if (!(r_iter->type & next_type)) {
					continue;
				}

				auto match = matches.str(r_iter->group);
				s_iter += match.length();

				terms.push_back({
					.val = std::move(match),
					.type = r_iter->type,
					.priority = r_iter->priority,
				});

				if (r_iter->recursive_group != -1) {
					auto recursive_match = matches.str(r_iter->recursive_group);
					auto recursive_terms = parse(recursive_match);
					terms.insert(
						terms.end(),
						std::make_move_iterator(recursive_terms.begin()),
						std::make_move_iterator(recursive_terms.end())
					);
					s_iter += recursive_match.length();
				}

				prev_type = r_iter->type;

				if (r_iter->type & TYPE_BINARY_OPERATOR) {
					next_type = TYPE_EXPR | TYPE_UNARY_OPERATOR;
				} else if (r_iter->type & TYPE_UNARY_OPERATOR) {
					next_type = TYPE_EXPR;
				} else {
					next_type = TYPE_ANY;
				}

				break;
			}
		}

		if (r_iter == regexes.end()) {
			throw std::invalid_argument("Unknown term");
		}
	}

	return terms;
}

std::vector<Term> to_postfix(std::vector<Term> &terms)
{
	std::vector<Term> out;
	std::vector<Term> stack;

	for (auto term : terms) {
		if (term.type & (TYPE_VARIABLE | TYPE_CONST)) {
			out.push_back(std::move(term));
			continue;
		} else if (term.type & (TYPE_OPERATOR | TYPE_FUNCTION)) {
			stack.push_back(std::move(term));
			for (auto s_iter = stack.rbegin(); s_iter != stack.rend(); ++s_iter) {
				if (s_iter->priority > term.priority) {
					out.push_back(std::move(*s_iter));
					s_iter = std::reverse_iterator(stack.erase(std::next(s_iter).base()));
				}
			}
		} else if (term.type & TYPE_OPEN_BRACKET) {
			stack.push_back(std::move(term));
		} else if (term.type & TYPE_CLOSING_BRACKET) {
			for (auto s_iter = stack.rbegin(); s_iter != stack.rend(); ++s_iter) {
				if (s_iter->type != TYPE_OPEN_BRACKET) {
					out.push_back(std::move(*s_iter));
					s_iter = std::reverse_iterator(stack.erase(std::next(s_iter).base()));
				} else {
					stack.pop_back();
					break;
				}
			}
		}
	}

	return out;
}

typedef std::function<double(std::string, std::string)> BinaryOperator;
typedef std::function<double(std::string)> UnaryOperator;

std::map<std::string, BinaryOperator> binary_operators = {
	{ "+",
	  [](std::string _op1, std::string _op2) -> double {
		  double op1 = std::stod(_op1);
		  double op2 = std::stod(_op2);
		  return op1 + op2;
	  } }
};

std::map<std::string, UnaryOperator> unary_operators = {
	{
		"abs",
		[](std::string _op) -> double {
			double op = std::stod(_op);
			return std::abs(op);
		},
	},
	{ "-",
	  [](std::string _op) -> double {
		  double op = std::stod(_op);
		  return -op;
	  } }
};

double calc(std::vector<Term> postfix)
{
	double result = 0;
	std::vector<Term> stack;

	for (auto term : postfix) {
		if (term.type & TYPE_CONST) {
			stack.push_back(std::move(term));
		} else if (term.type & TYPE_BINARY_OPERATOR) {
			auto op1 = std::move(stack.back());
			stack.pop_back();
			auto op2 = std::move(stack.back());
			stack.pop_back();

			auto oper = binary_operators[term.val];
			double oper_result = binary_operators[term.val](op1.val, op2.val);
		} else if (term.type & TYPE_UNARY_OPERATOR) {
			auto op = std::move(stack.back());
			stack.pop_back();
		}
	}
}

int main()
{
	std::string str = "x+111-abs(-234)";
	std::vector<Term> terms = parse(str);
	std::vector<Term> postfix = to_postfix(terms);

	for (auto term : postfix) {
		std::cout << term.val;
	}

	return 0;
}
