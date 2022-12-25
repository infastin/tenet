#ifndef CALCULATOR_HPP_MOJED3IQ
#define CALCULATOR_HPP_MOJED3IQ

#include "evaluator.hpp"

#include <set>

class Calculator : Evaluator {
public:
	Calculator(
		const std::vector<Parser::BrickRegex> &regexes,
		const std::map<std::string, Evaluator::Operator> &operators,
		const std::map<std::string, Evaluator::Operator> &functions,
		const std::map<std::string, double> &variables
	)
		: Evaluator(operators, functions, variables)
		, regexes(regexes){};

	template<typename... Args>
	double calc(const std::string &expr, Args... args)
	{
		auto bricks = Parser::parse(regexes, expr);
		auto postfix = Parser::to_postfix(bricks);

		std::vector<double> var_array = { args... };
		std::set<std::string> expected;
		std::map<std::string, double> vars;

		for (auto &term : postfix) {
			if (term.type & Parser::Brick::VARIABLE && !variables.contains(term.val)) {
				expected.insert(term.val);
			}
		}

		if (expected.size() > var_array.size()) {
			throw std::logic_error("Not enough variables passed");
		}

		size_t i = 0;
		for (auto iter : expected) {
			vars.insert({ iter, var_array[i++] });
		}

		return evaluate(postfix, vars);
	}

	template<typename... Args>
	double operator()(const std::string &expr, Args... args)
	{
		return calc(expr, args...);
	}

private:
	std::vector<Parser::BrickRegex> regexes;
};

#endif /* end of include guard: CALCULATOR_HPP_MOJED3IQ */
