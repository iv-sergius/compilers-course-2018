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
	// TODO: add other tokens here.
};

struct Token
{
	TokenType type = TT_END;
	std::optional<std::string> value;
};

}
