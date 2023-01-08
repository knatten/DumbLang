#include "lexer.h"

#include <sstream>

int main()
{
    std::stringstream ss1{"let x = 0\nlet y = 1"};
    lexer::print_tokens(lexer::lex(ss1));
    std::stringstream ss2;
    lexer::print_tokens(lexer::lex(ss2));
}
