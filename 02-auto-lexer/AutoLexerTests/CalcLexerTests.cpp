#include <catch2/catch.hpp>
#include "../AutoLexer/CalcLexer.h"
#include <vector>

using namespace calc;

namespace calc
{
bool operator ==(const Token& a, const Token& b)
{
	return a.type == b.type && a.value == b.value;
}

boost::string_view GetTokenName(TokenType type)
{
	switch (type)
	{
	case calc::TT_END:
		return "end";
	case calc::TT_ERROR:
		return "error";
	case calc::TT_NUMBER:
		return "number";
	case calc::TT_ID:
		return "id";
	case calc::TT_PLUS:	
		return "+";
	case calc::TT_MINUS:
		return "-";
	case calc::TT_ASTERISK:
		return "*";
	case calc::TT_SLASH:
		return "/";
	case calc::TT_EQUAL:
		return "=";
	case calc::TT_OPEN_BRACKET:
		return "(";
	case calc::TT_CLOSE_BRACKET:
		return ")";
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

using TokenList = std::vector<Token>;

TokenList Tokenize(boost::string_view text)
{
	TokenList results;
	CalcLexer lexer{ text };
	for (Token token = lexer.Read(); token.type != TT_END; token = lexer.Read())
	{
		results.emplace_back(std::move(token));
	}
	return results;
}

}

TEST_CASE("Can read one number", "[CalcLexer]") {
	REQUIRE(Tokenize("0") == TokenList{
		Token{ TT_NUMBER, "0" },
		});
	REQUIRE(Tokenize("1") == TokenList{
		Token{ TT_NUMBER, "1" },
		});
	REQUIRE(Tokenize("9876543210") == TokenList{
		Token{ TT_NUMBER, "9876543210" },
		});
}

TEST_CASE("Can read one operator", "[CalcLexer]") {
	REQUIRE(Tokenize("+") == TokenList{
		Token{ TT_PLUS },
		});
	REQUIRE(Tokenize("-") == TokenList{
		Token{ TT_MINUS },
		});
	REQUIRE(Tokenize("*") == TokenList{
		Token{ TT_ASTERISK },
		});
	REQUIRE(Tokenize("/") == TokenList{
		Token{ TT_SLASH },
		});
}

TEST_CASE("Can read one bracket", "[CalcLexer]") {
	REQUIRE(Tokenize("(") == TokenList{
		Token{ TT_OPEN_BRACKET },
		});
	REQUIRE(Tokenize(")") == TokenList{
		Token{ TT_CLOSE_BRACKET },
		});
}

TEST_CASE("Can read one id", "[CalcLexer]") {
	REQUIRE(Tokenize("a") == TokenList{
		Token{ TT_ID, "a" },
		});
	REQUIRE(Tokenize("A") == TokenList{
		Token{ TT_ID, "A" },
		});
	REQUIRE(Tokenize("_") == TokenList{
		Token{ TT_ID, "_" },
		});
	REQUIRE(Tokenize("a12") == TokenList{
		Token{ TT_ID, "a12" },
		});
	REQUIRE(Tokenize("A_1_b") == TokenList{
		Token{ TT_ID, "A_1_b" },
		});
	REQUIRE(Tokenize("_1B") == TokenList{
		Token{ TT_ID, "_1B" },
		});
	REQUIRE(Tokenize("1B_") == TokenList{
		Token{ TT_ERROR, "1B_" },
		});
}

TEST_CASE("Can read one equal", "[CalcLexer]") {
	REQUIRE(Tokenize("=") == TokenList{
		Token{ TT_EQUAL },
		});
}

TEST_CASE("Can read expression tokens", "[CalcLexer]") {
	REQUIRE(Tokenize("45+9+28") == TokenList{
		Token{ TT_NUMBER, "45" },
		Token{ TT_PLUS },
		Token{ TT_NUMBER, "9" },
		Token{ TT_PLUS },
		Token{ TT_NUMBER, "28" },
		});
#if 1 // fractional number support
	REQUIRE(Tokenize("5+7.005") == TokenList{
		Token{ TT_NUMBER, "5" },
		Token{ TT_PLUS },
		Token{ TT_NUMBER, "7.005" },
		});
	REQUIRE(Tokenize("1.005+43.54+1") == TokenList{
		Token{ TT_NUMBER, "1.005" },
		Token{ TT_PLUS },
		Token{ TT_NUMBER, "43.54" },
		Token{ TT_PLUS },
		Token{ TT_NUMBER, "1" },
		});
	REQUIRE(Tokenize("5.") == TokenList{
		Token{ TT_ERROR, "5." },
		});
	REQUIRE(Tokenize(".4") == TokenList{
		Token{ TT_ERROR, ".4" },
		});
	REQUIRE(Tokenize("5..0") == TokenList{
		Token{ TT_ERROR, "5..0" },
		});
	REQUIRE(Tokenize("5.00.0") == TokenList{
		Token{ TT_ERROR, "5.00.0" },
		});
#endif
}

TEST_CASE("Can read one operator with whitespaces", "[CalcLexer]") {
	REQUIRE(Tokenize("  +") == TokenList{
		Token{ TT_PLUS },
		});
	REQUIRE(Tokenize("\t+") == TokenList{
		Token{ TT_PLUS },
		});
	REQUIRE(Tokenize("   \t\t+") == TokenList{
		Token{ TT_PLUS },
		});
	REQUIRE(Tokenize("\n+") == TokenList{
		Token{ TT_PLUS },
		});
	REQUIRE(Tokenize("   \n  +") == TokenList{
		Token{ TT_PLUS },
		});
	REQUIRE(Tokenize("\t   \n  +") == TokenList{
		Token{ TT_PLUS },
		});
	REQUIRE(Tokenize("+    ") == TokenList{
		Token{ TT_PLUS },
		});
	REQUIRE(Tokenize("+  \t\t   ") == TokenList{
		Token{ TT_PLUS },
		});
	REQUIRE(Tokenize("+  \n\t   ") == TokenList{
		Token{ TT_PLUS },
		});
	REQUIRE(Tokenize("   +   ") == TokenList{
		Token{ TT_PLUS },
		});
	REQUIRE(Tokenize("  \t +  \t ") == TokenList{
		Token{ TT_PLUS },
		});
	REQUIRE(Tokenize("  \t\t +  \t ") == TokenList{
		Token{ TT_PLUS },
		});
	REQUIRE(Tokenize("  \t \t +  \n\t ") == TokenList{
		Token{ TT_PLUS },
		});
}

TEST_CASE("Error on number with 0 on first position", "[CalcLexer]") {
	REQUIRE(Tokenize("00") == TokenList{
		Token{ TT_ERROR, "00" },
		});
	REQUIRE(Tokenize("0") == TokenList{
		Token{ TT_NUMBER, "0" },
		});
	REQUIRE(Tokenize("05") == TokenList{
		Token{ TT_ERROR, "05" },
		});
	REQUIRE(Tokenize("0.123") == TokenList{
		Token{ TT_NUMBER, "0.123" },
		});
}

TEST_CASE("Can read one number with whitespaces", "[CalcLexer]") {
	REQUIRE(Tokenize("  1") == TokenList{
		Token{ TT_NUMBER, "1" },
		});
	REQUIRE(Tokenize("\t4") == TokenList{
		Token{ TT_NUMBER, "4" },
		});
	REQUIRE(Tokenize("   \t\t3.2") == TokenList{
		Token{ TT_NUMBER, "3.2" },
		});
	REQUIRE(Tokenize("\n9") == TokenList{
		Token{ TT_NUMBER, "9" },
		});
	REQUIRE(Tokenize("   \n  15") == TokenList{
		Token{ TT_NUMBER, "15" },
		});
	REQUIRE(Tokenize("\t   \n  21.03") == TokenList{
		Token{ TT_NUMBER, "21.03" },
		});
	REQUIRE(Tokenize("0    ") == TokenList{
		Token{ TT_NUMBER, "0" },
		});
	REQUIRE(Tokenize("81  \t\t   ") == TokenList{
		Token{ TT_NUMBER, "81" },
		});
	REQUIRE(Tokenize("4.2  \n\t   ") == TokenList{
		Token{ TT_NUMBER, "4.2" },
		});
	REQUIRE(Tokenize("   7.9   ") == TokenList{
		Token{ TT_NUMBER, "7.9" },
		});
	REQUIRE(Tokenize("  \t 3  \t ") == TokenList{
		Token{ TT_NUMBER, "3" },
		});
	REQUIRE(Tokenize("  \t\t 9.001  \t ") == TokenList{
		Token{ TT_NUMBER, "9.001" },
		});
	REQUIRE(Tokenize("  \t \t 7  \n\t ") == TokenList{
		Token{ TT_NUMBER, "7" },
		});
}

TEST_CASE("Can read expression tokens with whitespaces") {
	REQUIRE(Tokenize("2 + 3") == TokenList{
		Token{ TT_NUMBER, "2" },
		Token{ TT_PLUS },
		Token{ TT_NUMBER, "3" },
		});
	REQUIRE(Tokenize("\t0.52 + \n4") == TokenList{
		Token{ TT_NUMBER, "0.52" },
		Token{ TT_PLUS },
		Token{ TT_NUMBER, "4" },
		});
	REQUIRE(Tokenize("\n+ \t7.1") == TokenList{
		Token{ TT_PLUS },
		Token{ TT_NUMBER, "7.1" },
		});

}

TEST_CASE("Cannot read number which starts with zero") {
	REQUIRE(Tokenize("0123456789") == TokenList{
		Token{ TT_ERROR, "0123456789" },
		});
	REQUIRE(Tokenize("01.25") == TokenList{
		Token{ TT_ERROR, "01.25" },
		});
	REQUIRE(Tokenize("+01") == TokenList{
		Token{ TT_PLUS },
		Token{ TT_ERROR, "01" },
		});
	REQUIRE(Tokenize("+00.32") == TokenList{
		Token{ TT_PLUS },
		Token{ TT_ERROR, "00.32" },
		});
	REQUIRE(Tokenize("4+0521") == TokenList{
		Token{ TT_NUMBER, "4" },
		Token{ TT_PLUS },
		Token{ TT_ERROR, "0521" },
		});
	REQUIRE(Tokenize("02+21") == TokenList{
		Token{ TT_ERROR, "02" },
		Token{ TT_PLUS },
		Token{ TT_NUMBER, "21" },
		});
	REQUIRE(Tokenize("02.4+5.3") == TokenList{
		Token{ TT_ERROR, "02.4" },
		Token{ TT_PLUS },
		Token{ TT_NUMBER, "5.3" },
		});
}

TEST_CASE("Can read statment with whitespaces", "[CalcLexer]") {
	REQUIRE(Tokenize("a = 1") == TokenList{
		Token{ TT_ID, "a" },
		Token{ TT_EQUAL },
		Token{ TT_NUMBER, "1" }
		});
	REQUIRE(Tokenize(" var=7* (1 + 6)") == TokenList{
		Token{ TT_ID, "var" },
		Token{ TT_EQUAL },
		Token{ TT_NUMBER, "7" },
		Token{ TT_ASTERISK },
		Token{ TT_OPEN_BRACKET },
		Token{ TT_NUMBER, "1" },
		Token{ TT_PLUS },
		Token{ TT_NUMBER, "6" },
		Token{ TT_CLOSE_BRACKET },
		});	
	REQUIRE(Tokenize("  k = 4 / (8.ab + .1) - 5abc * 9 ") == TokenList{
		Token{ TT_ID, "k" },
		Token{ TT_EQUAL },
		Token{ TT_NUMBER, "4" },
		Token{ TT_SLASH },
		Token{ TT_OPEN_BRACKET },
		Token{ TT_ERROR, "8.ab" },
		Token{ TT_PLUS },
		Token{ TT_ERROR, ".1" },
		Token{ TT_CLOSE_BRACKET },
		Token{ TT_MINUS },
		Token{ TT_ERROR, "5abc" },
		Token{ TT_ASTERISK },
		Token{ TT_NUMBER, "9" },
		});
}
