#include "parser.hpp"

#include <iostream>

Parser::BrickRegexBuilder::BrickRegexBuilder(const std::string &re_str, Brick::Type type)
{
	regex.re = std::regex(re_str);
	regex.val = "";
	regex.props = BrickRegex::USE_GROUP;
	regex.type = type;
	regex.priority = 0;
	regex.recursive_group = 0;
	regex.group = 0;
}

Parser::BrickRegexBuilder &Parser::BrickRegexBuilder::priority(int priority)
{
	regex.priority = priority;
	return *this;
}

Parser::BrickRegexBuilder &Parser::BrickRegexBuilder::val(const std::string &val)
{
	regex.val = val;
	regex.props &= ~BrickRegex::USE_GROUP;
	regex.props |= BrickRegex::USE_VALUE;
	return *this;
}

Parser::BrickRegexBuilder &Parser::BrickRegexBuilder::group(int group)
{
	regex.group = group;
	regex.props &= ~BrickRegex::USE_VALUE;
	regex.props |= BrickRegex::USE_GROUP;
	return *this;
}

Parser::BrickRegexBuilder &Parser::BrickRegexBuilder::recursion(int recursive_group)
{
	regex.recursive_group = recursive_group;
	regex.props |= BrickRegex::USE_RECURSION;
	return *this;
}

Parser::BrickRegex &&Parser::BrickRegexBuilder::build()
{
	return std::move(regex);
}

Parser::Parser(const std::vector<BrickRegex> &regexes)
{
	this->regexes = regexes;
}

std::vector<Parser::Brick> Parser::parse(const std::string &str)
{
	std::vector<Brick> bricks;

	int prev_type = Brick::NONE;
	int next_type = Brick::UNARY_OPERATOR | Brick::EXPR;

	for (auto s_iter = str.cbegin(), s_end = str.cend(); s_iter != s_end;) {
		std::smatch matches;

		auto r_iter = regexes.begin();
		for (; r_iter != regexes.end(); ++r_iter) {
			if (std::regex_search(s_iter, s_end, matches, r_iter->re, std::regex_constants::match_continuous)) {
				if (r_iter->type == Brick::UNARY_OPERATOR && (prev_type & Brick::EXPR)) {
					continue;
				} else if (r_iter->type == Brick::BINARY_OPERATOR && !(prev_type & Brick::EXPR)) {
					continue;
				} else if (!(r_iter->type & next_type)) {
					continue;
				}

				auto match = matches.str(r_iter->group);
				s_iter += matches.str().size();

				std::string val;
				if (r_iter->props & BrickRegex::USE_GROUP) {
					val = std::move(match);
				} else if (r_iter->props & BrickRegex::USE_VALUE) {
					val = r_iter->val;
				}

				std::vector<Brick> recursive_bricks;
				if (r_iter->props & BrickRegex::USE_RECURSION) {
					auto recursive_match = matches.str(r_iter->recursive_group);
					recursive_bricks = parse(recursive_match);
				}

				bricks.push_back(Brick{
					.bricks = std::move(recursive_bricks),
					.val = std::move(val),
					.type = r_iter->type,
					.priority = r_iter->priority,
				});

				prev_type = r_iter->type;

				if (r_iter->type & Brick::BINARY_OPERATOR) {
					next_type = Brick::EXPR | Brick::UNARY_OPERATOR;
				} else if (r_iter->type & Brick::UNARY_OPERATOR) {
					next_type = Brick::EXPR;
				} else {
					next_type = Brick::ANY;
				}

				break;
			}
		}

		if (r_iter == regexes.end()) {
			throw std::logic_error("Unknown brick at position " + std::to_string(std::distance(str.cbegin(), s_iter)));
		}
	}

	return bricks;
}
