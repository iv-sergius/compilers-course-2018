#include "CalcLexer.h"

namespace calc
{
namespace
{
bool IsDigit(char ch)
{
	/*
	 * Returns true if given character is digit.
	 */
	switch (ch)
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		return true;
	default:
		return false;
	}
}
bool IsSpace(char ch)
{
	/*
	 * Returns true if given character is space.
	 */
	switch (ch)
	{
	case ' ':
	case '\t':
	case '\n':
		return true;
	default:
		return false;
	}
}
}

CalcLexer::CalcLexer(std::string_view sources)
	: m_sources(sources)
{
}

Token CalcLexer::Read()
{
	/*
     * Reads next token from input string with following steps:
	 * 1) skips whitespace characters
	 * 2) checks for the end of input
	 * 3) checks first character to select token type
	 * 4) if token may have several characters, read them all
	 */

	SkipSpaces();

	if (m_position >= m_sources.size())
	{
		return Token{ TT_END };
	}

	char next = m_sources[m_position];
	++m_position;

	switch (next)
	{
	case '+':
		return Token{ TT_PLUS };
	case '-':
		return Token{ TT_MINUS };
	case '*':
		return Token{ TT_MULTIPLICATION };
	case '/':
		return Token{ TT_DIVISION };
	case '(':
		return Token{ TT_OPEN_BRACKET };
	case ')':
		return Token{ TT_CLOSE_BRACKET };
	default:
		break;
	}

	if (IsDigit(next))
	{
		return ReadNumber(next);
	}

	return Token{ TT_ERROR };
}

void CalcLexer::SkipSpaces()
{
	// TODO: skip whitespace characters - at least ' ', '\t' and '\n'.
	while (m_position < m_sources.size() && IsSpace(m_sources[m_position]))
	{
		++m_position;
	}
}

Token CalcLexer::ReadNumber(char head)
{
	/*
	 * Reads the tail of number token and returns this token.
	 * PRECONDITION: first character already read.
	 * POSTCONDITION: all number characters have been read.
	 */
	std::string value;
	value += head;
	bool wasError = false;

	if (head == '0' && m_position < m_sources.size() && IsDigit(m_sources[m_position])) {
		wasError = true;
	}
	while (m_position < m_sources.size() && IsDigit(m_sources[m_position]))
	{
		value += m_sources[m_position];
		++m_position;
	}
	if (m_position < m_sources.size() && m_sources[m_position] == '.')
	{
		value += m_sources[m_position];
		++m_position;
		while (m_position < m_sources.size() && IsDigit(m_sources[m_position]))
		{
			value += m_sources[m_position];
			++m_position;
		}
		if (m_position < m_sources.size() && m_sources[m_position] == '.')
		{
			wasError = true;
			// read til end
			while (m_position < m_sources.size() && (IsDigit(m_sources[m_position]) || m_sources[m_position] == '.'))
			{
				value += m_sources[m_position];
				++m_position;
			}
		}
	}
	if (value[value.length() - 1] == '.') {
		wasError = true;
	}
	if (wasError)
	{
		return Token{ TT_ERROR, value };
	}
	else
	{
		return Token{ TT_NUMBER, value };
	}
}

}
