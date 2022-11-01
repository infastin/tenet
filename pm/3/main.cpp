#include "evaluator.hpp"
#include "parser.hpp"

#include <cmath>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <vector>

static const std::vector<Parser::BrickRegex> regexes = {
	Parser::BrickRegexBuilder("\\d+(?:\\.\\d+)?", Parser::Brick::CONST).build(),
	Parser::BrickRegexBuilder("(\\w+)\\((.*)\\)", Parser::Brick::FUNCTION).group(1).priority(4).recursion(2).build(),
	Parser::BrickRegexBuilder("\\w+", Parser::Brick::VARIABLE).build(),
	Parser::BrickRegexBuilder("\\s+", Parser::Brick::NONE).build(),
	Parser::BrickRegexBuilder("\\((.*)\\)", Parser::Brick::BRACKETS).recursion(1).val().build(),
	Parser::BrickRegexBuilder("\\-", Parser::Brick::UNARY_OPERATOR).priority(5).val("um").build(),
	Parser::BrickRegexBuilder("\\+|-", Parser::Brick::BINARY_OPERATOR).priority(1).build(),
	Parser::BrickRegexBuilder("\\^", Parser::Brick::BINARY_OPERATOR).priority(3).build(),
	Parser::BrickRegexBuilder("\\*|\\/|%", Parser::Brick::BINARY_OPERATOR).priority(2).build(),
};

static std::map<std::string, double> variables = {
	{ "pi", M_PI },
	{ "e", M_E },
};

static std::map<std::string, Evaluator::Operator> operators = {
	{ "+", [](std::stack<double> &op_stack) -> double {
		 double op2 = op_stack.top();
		 op_stack.pop();
		 double op1 = op_stack.top();
		 op_stack.pop();
		 return op1 + op2;
	 } },
	{ "um", [](std::stack<double> &op_stack) -> double {
		 double op = op_stack.top();
		 op_stack.pop();
		 return -op;
	 } },
	{ "-", [](std::stack<double> &op_stack) -> double {
		 double op2 = op_stack.top();
		 op_stack.pop();
		 double op1 = op_stack.top();
		 op_stack.pop();
		 return op1 - op2;
	 } },
	{ "^", [](std::stack<double> &op_stack) -> double {
		 double op2 = op_stack.top();
		 op_stack.pop();
		 double op1 = op_stack.top();
		 op_stack.pop();
		 return std::pow(op1, op2);
	 } },
	{ "/", [](std::stack<double> &op_stack) -> double {
		 double op2 = op_stack.top();
		 op_stack.pop();
		 double op1 = op_stack.top();
		 op_stack.pop();
		 return op1 / op2;
	 } },
	{ "%", [](std::stack<double> &op_stack) -> double {
		 double op2 = op_stack.top();
		 op_stack.pop();
		 double op1 = op_stack.top();
		 op_stack.pop();
		 return std::fmod(op1, op2);
	 } },
	{ "*", [](std::stack<double> &op_stack) -> double {
		 double op2 = op_stack.top();
		 op_stack.pop();
		 double op1 = op_stack.top();
		 op_stack.pop();
		 return op1 * op2;
	 } }
};

static std::map<std::string, Evaluator::Operator> functions = {
	{ "sqrt", [](std::stack<double> &op_stack) -> double {
		 double op = op_stack.top();
		 op_stack.pop();
		 return std::sqrt(op);
	 } },
	{ "abs", [](std::stack<double> &op_stack) -> double {
		 double op = op_stack.top();
		 op_stack.pop();
		 return std::fabs(op);
	 } },
	{ "sin", [](std::stack<double> &op_stack) -> double {
		 double op = op_stack.top();
		 op_stack.pop();
		 return std::sin(op);
	 } },
	{ "cos", [](std::stack<double> &op_stack) -> double {
		 double op = op_stack.top();
		 op_stack.pop();
		 return std::cos(op);
	 } },
};

void print_bricks(std::vector<Parser::Brick> &terms)
{
	for (auto &term : terms) {
		if (term.type & Parser::Brick::BRACKETS) {
			std::cout << "(";
			print_bricks(term.bricks);
			std::cout << ")";
		} else if (term.type & Parser::Brick::FUNCTION) {
			std::cout << term.val;
			std::cout << "(";
			print_bricks(term.bricks);
			std::cout << ")";
		} else {
			std::cout << term.val;
		}
	}
}

void print_terms(std::vector<Evaluator::Term> &terms)
{
	for (auto &term : terms) {
		std::cout << term.val;
	}
}

void read_variables(std::set<std::string> &should_read, std::map<std::string, double> &variables)
{
	std::regex re("(\\w+)\\s*=\\s*(-?\\d+(?:\\.\\d+)?)");
	std::smatch matches;
	std::string line;

	size_t i = 0;
	for (; i < should_read.size() && std::getline(std::cin, line); ++i) {
		if (!std::regex_match(line, matches, re)) {
			throw std::logic_error("Incorrect format on line " + std::to_string(i));
		}

		std::string variable = matches.str(1);
		std::string value = matches.str(2);

		variables.insert({ variable, std::stod(value) });
	}

	if (i != should_read.size()) {
		throw std::logic_error("Not enough variables read");
	}

	for (auto &var : should_read) {
		if (!variables.contains(var)) {
			throw std::logic_error("Variable '" + var + "' isn't specified");
		}
	}
}

int main()
{
	Parser parser(regexes);

	std::string str;
	std::getline(std::cin, str);

	auto terms = parser.parse(str);
	auto postfix = Evaluator::to_postfix(terms);

	std::set<std::string> should_read;
	for (auto &term : postfix) {
		if (term.type & Parser::Brick::VARIABLE && !variables.contains(term.val)) {
			should_read.insert(term.val);
		}

		if (term.type & Parser::Brick::FUNCTION && !functions.contains(term.val)) {
			throw std::logic_error("Function '" + term.val + "' isn't defined");
		}
	}

	read_variables(should_read, variables);
	Evaluator evaluator(operators, functions, variables);
	std::cout << evaluator.evaluate(postfix) << std::endl;

	return 0;
}
