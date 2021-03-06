#include "CalcLexer.h"

using namespace calc;

CalcLexer::CalcLexer(const boost::string_view sources)
{
	lexertl::rules rules;
	m_autoLexer = std::make_unique<lexertl::state_machine>();

	rules.push("[a-zA-Z_][a-zA-Z0-9_]*", calc::TT_ID);
	rules.push("(0|[1-9][0-9]*)(\\.[0-9]+)?", calc::TT_NUMBER);
	rules.push("[a-zA-Z0-9_\\.]+", calc::TT_ERROR);
	rules.push("\\+", calc::TT_PLUS);
	rules.push("\\-", calc::TT_MINUS);
	rules.push("\\*", calc::TT_ASTERISK);
	rules.push("\\/", calc::TT_SLASH);
	rules.push("\\=", calc::TT_EQUAL);
	rules.push("\\(", calc::TT_OPEN_BRACKET);
	rules.push("\\)", calc::TT_CLOSE_BRACKET);

	rules.push("[ \t\r\n]+", rules.skip());

	lexertl::generator::build(rules, *m_autoLexer);

	//m_autoLexer->minimise();

	m_stateIter = lexertl::citerator(sources.begin(), sources.end(), *m_autoLexer);

}

Token CalcLexer::Read()
{
	lexertl::citerator end;
	if (m_stateIter != end)
	{
		Token result(m_stateIter->id, m_stateIter->str());
		++m_stateIter;
		return result;
	}
	else
	{
		return { TT_END };
	}
}