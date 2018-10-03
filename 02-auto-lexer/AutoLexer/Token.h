#pragma once

#include <string>
#include <boost/optional.hpp>

namespace calc
{

enum TokenType
{
	TT_END = 0,
	TT_ERROR = 1,
	TT_NUMBER,
	TT_ID,
	TT_PLUS,
	TT_MINUS,
	TT_ASTERISK,
	TT_SLASH,
	TT_EQUAL,
	TT_OPEN_BRACKET,
	TT_CLOSE_BRACKET,
};

struct Token
{
	TokenType type = TT_END;
	boost::optional<std::string> value;
	
	Token(TokenType type, std::string str)
		:type(type), value(str)
	{}
	Token(size_t number, std::string str)
	{
		if (number > 0 && number <= TokenType::TT_CLOSE_BRACKET)
		{
			type = static_cast<TokenType>(number);
		}
		else
		{
			type = TT_ERROR;
		}
		if (type == TT_NUMBER || type == TT_ID || type == TT_ERROR)
		{
			value = str;
		}
	}
	Token(TokenType type)
		:type(type), value()
	{}
};


}
