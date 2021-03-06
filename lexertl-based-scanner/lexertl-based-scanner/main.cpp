#include "lexertl/generator.hpp"
#include "lexertl/iterator.hpp"
#include <boost/range/iterator_range.hpp>
#include <iostream>
#include <memory>

enum {
	// ID пользовательских токенов должны начинаться с 1,
	//  т.к. число 0 lexertl резервирует для ID "конец ввода".
	// Если один из ID равен нулю, программа аварийно завершится.
	TK_NUMBER = 1,
	TK_IDENTIFIER,
	TK_PLUS,
	TK_MINUS,
	TK_MULTIPLY,
	TK_DIVIDE,
};

// Создаёт детерминированный конечный автомат (DFA)
//  для лексического анализа грамматики калькулятора
std::unique_ptr<lexertl::state_machine> BuildCalcLexer()
{
	lexertl::rules rules;

	// Метод push добавляет правило
	rules.push("[0-9]+", TK_NUMBER);
	rules.push("[a-z]+", TK_IDENTIFIER);
	rules.push("\\+", TK_PLUS);
	rules.push("\\-", TK_MINUS);
	rules.push("\\*", TK_MULTIPLY);
	rules.push("\\/", TK_DIVIDE);

	// Метод skip возвращает специальный ID,
	//  который означает, что это совпадение игнорируется.
	// Так мы можем пропустить пробельные символы.
	rules.push("[ \t\r\n]+", rules.skip());

	auto lexer = std::make_unique<lexertl::state_machine>();
	lexertl::generator::build(rules, *lexer);

	return lexer;
}

// Возвращает диапазон для прохода слева направо,
//  который при запросе следующего элемента
//  получает его из строки `line` с помощью лексера `lexer`
decltype(auto) TokenizeLine(const lexertl::state_machine &lexer, std::string &line)
{
	lexertl::siterator begin(line.begin(), line.end(), lexer);
	lexertl::siterator end;

	// Непосредственно при вызове токенизации не происходит,
	//  такой подход называется lazy evaluation with generators.
	return boost::make_iterator_range(begin, end);
}

int main()
{
	auto lexer = BuildCalcLexer();

	while (std::cin)
	{
		std::string line;
		std::getline(std::cin, line);
		for (auto &token : TokenizeLine(*lexer, line))
		{
			std::cout << "Id: " << token.id << ", Token: '" << token.str() << "'\n";
		}
	}

	return 0;
}