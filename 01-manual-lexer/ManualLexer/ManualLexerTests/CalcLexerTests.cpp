#include "catch2/catch.hpp"
#include "../ManualLexer/CalcLexer.h"
#include <vector>

using namespace std;
using namespace calc;

namespace calc
{
bool operator ==(const Token& a, const Token& b)
{
	return a.type == b.type && a.value == b.value;
}

string_view GetTokenName(TokenType type)
{
	switch (type)
	{
	case calc::TT_END:
		return "end";
	case calc::TT_ERROR:
		return "error";
	case calc::TT_NUMBER:
		return "number";
	case calc::TT_PLUS:
		return "+";
	}
	return "<UNEXPECTED!!!>";
}

std::ostream& operator<<(std::ostream& stream, const Token& token)
{
	stream << "Token(" << GetTokenName(token.type);
	if (token.value)
	{
		stream << ", " << *token.value;
	}
	stream << ")";
	return stream;
}
}

namespace
{

using TokenList = vector<Token>;

TokenList Tokenize(string_view text)
{
	TokenList results;
	CalcLexer lexer{ text };
	for (Token token = lexer.Read(); token.type != TT_END; token = lexer.Read())
	{
		results.emplace_back(move(token));
	}
	return results;
}

}

TEST_CASE("Can parse one number", "[CalcLexer]") {
	REQUIRE(Tokenize("0"sv) == TokenList{
		Token{ TT_NUMBER, "0"s },
		});
	REQUIRE(Tokenize("1"sv) == TokenList{
		Token{ TT_NUMBER, "1"s },
		});
	REQUIRE(Tokenize("9876543210"sv) == TokenList{
		Token{ TT_NUMBER, "9876543210"s },
		});
}

TEST_CASE("Can parse one operator", "[CalcLexer]") {
	REQUIRE(Tokenize("+"sv) == TokenList{
		Token{ TT_PLUS },
		});
}

#if 0 // whitespace support
// TODO: this test should pass OK.
TEST_CASE("Can parse one operator after whitespace", "[CalcLexer]") {
	REQUIRE(Tokenize("  +"sv) == TokenList{
		Token{ TT_PLUS },
		});
	REQUIRE(Tokenize("\t+"sv) == TokenList{
		Token{ TT_PLUS },
		});
	REQUIRE(Tokenize("   \t\t+"sv) == TokenList{
		Token{ TT_PLUS },
		});
}
#endif

#if 0 // extended numbers support
// TODO: this test should pass OK.
TEST_CASE("Cannot read number which starts with zero") {
	REQUIRE(Tokenize("0123456789"sv) == TokenList{
		Token{ TT_ERROR },
		});
}
#endif
