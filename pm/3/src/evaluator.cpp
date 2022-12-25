#include "evaluator.hpp"

#include <iostream>
#include <string>

Evaluator::Evaluator(
	const std::map<std::string, Operator> &operators,
	const std::map<std::string, Operator> &functions,
	const std::map<std::string, double> &variables
)
{
	this->operators = operators;
	this->functions = functions;
	this->variables = variables;
}

double Evaluator::evaluate(
	const std::vector<Parser::Term> &postfix,
	const std::map<std::string, double> &variables
)
{
	for (auto &term : postfix) {
		if (term.type & Parser::Brick::CONST) {
			op_stack.push(std::stod(term.val));
		} else if (term.type & Parser::Brick::VARIABLE) {
			if (this->variables.contains(term.val)) {
				op_stack.push(this->variables[term.val]);
			} else if (variables.contains(term.val)) {
				op_stack.push(variables.at(term.val));
			} else {
				throw std::logic_error("Variable '" + term.val + "' isn't defined");
			}
		} else if (term.type & Parser::Brick::OPERATOR) {
			if (!operators.contains(term.val)) {
				throw std::logic_error("Operator '" + term.val + "' isn't defined");
			}

			auto oper = operators[term.val];
			double oper_result = oper(op_stack);
			op_stack.push(oper_result);
		} else if (term.type & Parser::Brick::FUNCTION) {
			if (!functions.contains(term.val)) {
				throw std::logic_error("Function '" + term.val + "' isn't defined");
			}

			auto func = functions[term.val];
			double func_result = func(op_stack);
			op_stack.push(func_result);
		}
	}

	double result = op_stack.top();
	op_stack.pop();

	return result;
}
