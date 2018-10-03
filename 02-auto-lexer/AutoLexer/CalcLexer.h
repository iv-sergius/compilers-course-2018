#pragma once

#include <boost/utility/string_view.hpp>
#include <lexertl/generator.hpp>
#include <lexertl/iterator.hpp>
#include "Token.h"

namespace calc
{

class CalcLexer
{
public:
	CalcLexer(const boost::string_view sources);

	Token Read();

private:
	std::unique_ptr<lexertl::state_machine> m_autoLexer;
	lexertl::citerator m_stateIter;
};

}
