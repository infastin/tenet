#ifndef EVALUATOR_HPP_XFIG8QJK
#define EVALUATOR_HPP_XFIG8QJK

#include "parser.hpp"

class Evaluator {
public:
	struct Term {
		std::string val;
		Parser::Brick::Type type;
		int priority;
	};

	typedef std::function<double(std::stack<double> &op_stack)> Operator;

	Evaluator(
		const std::map<std::string, Operator> &operators,
		const std::map<std::string, Operator> &functions,
		const std::map<std::string, double> &variables
	);

	static std::vector<Term> to_postfix(const std::vector<Parser::Brick> &terms);

	double evaluate(const std::vector<Evaluator::Term> &postfix);

private:
	std::stack<double> op_stack;
	std::map<std::string, Operator> operators;
	std::map<std::string, Operator> functions;
	std::map<std::string, double> variables;

	static void to_postfix(
		const std::vector<Parser::Brick> &terms,
		std::vector<Term> &stack,
		std::vector<Term> &out
	);
};

#endif /* end of include guard: EVALUATOR_HPP_XFIG8QJK */
