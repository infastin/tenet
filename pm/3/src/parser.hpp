#ifndef PARSER_HPP_GLRQIUEV
#define PARSER_HPP_GLRQIUEV

#include <regex>
#include <string>

class Parser {
public:
	struct Brick {
		enum Type {
			NONE = 0,
			CONST = 1,
			VARIABLE = 2,
			FUNCTION = 4,
			BINARY_OPERATOR = 8,
			UNARY_OPERATOR = 16,
			BRACKETS = 32,
			EXPR = CONST | VARIABLE | FUNCTION | BRACKETS,
			OPERATOR = BINARY_OPERATOR | UNARY_OPERATOR,
			ANY = EXPR | OPERATOR,
		};

		std::vector<Brick> bricks;
		std::string val;
		Type type;
		int priority;
	};

	struct BrickRegex {
		enum Props {
			NONE = 0,
			USE_GROUP = 1,
			USE_VALUE = 2,
			USE_RECURSION = 4,
		};

		std::regex re;
		std::string val;

		int props;
		Brick::Type type;
		int priority;
		int recursive_group;
		int group;
	};

	class BrickRegexBuilder {
		BrickRegex regex;

	public:
		BrickRegexBuilder(const std::string &re_str, Brick::Type type);
		BrickRegexBuilder &priority(int priority = 0);
		BrickRegexBuilder &val(const std::string &val = "");
		BrickRegexBuilder &group(int group = 0);
		BrickRegexBuilder &recursion(int recursive_group = 0);
		BrickRegex &&build();
	};

	Parser(const std::vector<BrickRegex> &regexes);

	std::vector<Brick> parse(const std::string &str);

private:
	std::vector<BrickRegex> regexes;
};

#endif /* end of include guard: PARSER_HPP_GLRQIUEV */
