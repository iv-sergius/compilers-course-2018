#pragma once

#include <string_view>
#include "Token.h"

namespace calc
{

class CalcLexer
{
public:
	CalcLexer(std::string_view sources);

	Token Read();

private:
	void SkipSpaces();
	Token ReadNumber(char head);
	Token ReadId(char head);

	std::string_view m_sources;
	size_t m_position = 0;
};

}
