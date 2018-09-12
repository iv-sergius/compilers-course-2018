#pragma once

#include <string>
#include <optional>

namespace calc
{

enum TokenType
{
	TT_END = 0,
	TT_ERROR,
	TT_NUMBER,
	TT_PLUS,
	TT_MINUS,
	TT_MULTIPLICATION,
	TT_DIVISION,
	TT_OPEN_BRACKET,
	TT_CLOSE_BRACKET,
};

struct Token
{
	TokenType type = TT_END;
	std::optional<std::string> value;
};

}
