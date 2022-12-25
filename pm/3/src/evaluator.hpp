#ifndef EVALUATOR_HPP_XFIG8QJK
#define EVALUATOR_HPP_XFIG8QJK

#include "parser.hpp"

class Evaluator {
public:
	typedef std::function<double(std::stack<double> &op_stack)> Operator;

	Evaluator(
		const std::map<std::string, Operator> &operators,
		const std::map<std::string, Operator> &functions,
		const std::map<std::string, double> &variables
	);

	double evaluate(
		const std::vector<Parser::Term> &postfix,
		const std::map<std::string, double> &variables = {}
	);

private:
	std::stack<double> op_stack;

protected:
	std::map<std::string, Operator> operators;
	std::map<std::string, Operator> functions;
	std::map<std::string, double> variables;
};

#endif /* end of include guard: EVALUATOR_HPP_XFIG8QJK */
