#include "CalcLexer.h"

using namespace calc;

CalcLexer::CalcLexer(std::string_view sources)
	: m_sources(sources)
{}

Token CalcLexer::Read()
{
	return { TT_END };
}

int main()
{}
