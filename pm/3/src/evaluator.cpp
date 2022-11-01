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

void Evaluator::to_postfix(
	const std::vector<Parser::Brick> &bricks,
	std::vector<Evaluator::Term> &stack,
	std::vector<Evaluator::Term> &out
)
{
	const size_t orig_size = stack.size();

	for (auto &brick : bricks) {
		if (brick.type & (Parser::Brick::VARIABLE | Parser::Brick::CONST)) {
			out.push_back(Evaluator::Term{
				.val = brick.val,
				.type = brick.type,
				.priority = brick.priority,
			});
		} else if (brick.type & Parser::Brick::OPERATOR) {
			while (stack.size() != orig_size && stack.back().priority >= brick.priority) {
				out.push_back(std::move(stack.back()));
				stack.pop_back();
			}

			stack.push_back(Evaluator::Term{
				.val = brick.val,
				.type = brick.type,
				.priority = brick.priority,
			});
		} else if (brick.type & (Parser::Brick::BRACKETS | Parser::Brick::FUNCTION)) {
			size_t cur_size = stack.size();
			to_postfix(brick.bricks, stack, out);
			while (stack.size() != cur_size) {
				out.push_back(std::move(stack.back()));
				stack.pop_back();
			}

			if (brick.type & Parser::Brick::FUNCTION) {
				stack.push_back(Evaluator::Term{
					.val = brick.val,
					.type = brick.type,
					.priority = brick.priority,
				});
			}
		}
	}
}

std::vector<Evaluator::Term> Evaluator::to_postfix(const std::vector<Parser::Brick> &terms)
{
	std::vector<Evaluator::Term> out;
	std::vector<Evaluator::Term> stack;

	to_postfix(terms, stack, out);

	while (!stack.empty()) {
		out.push_back(std::move(stack.back()));
		stack.pop_back();
	}

	return out;
}

double Evaluator::evaluate(const std::vector<Evaluator::Term> &postfix)
{
	for (auto &term : postfix) {
		if (term.type & Parser::Brick::CONST) {
			op_stack.push(std::stod(term.val));
		} else if (term.type & Parser::Brick::VARIABLE) {
			op_stack.push(variables[term.val]);
		} else if (term.type & Parser::Brick::OPERATOR) {
			auto oper = operators[term.val];
			double oper_result = oper(op_stack);
			op_stack.push(oper_result);
		} else if (term.type & Parser::Brick::FUNCTION) {
			auto func = functions[term.val];
			double func_result = func(op_stack);
			op_stack.push(func_result);
		}
	}

	double result = op_stack.top();
	op_stack.pop();

	return result;
}
