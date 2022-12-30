#include "lexer.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <variant>
#include <vector>

namespace lexer
{

    tokens::AnyToken to_token(const std::string &s)
    {
        if (s == "let")
            return tokens::Let{};
        if (s == "=")
            return tokens::Assignment{};
        if (s == "\n")
            return tokens::Newline{};
        if (std::all_of(s.cbegin(), s.cend(), ::isdigit))
        {
            return tokens::Literal{std::stoi(s)};
        }
        return tokens::Identifier{s};
    }

    std::vector<tokens::AnyToken> lex(std::istream &is)
    {
        std::vector<tokens::AnyToken> lexed_tokens;
        while (!is.eof())
        {
            const auto peek = is.peek();
            if (peek == std::istream::traits_type::eof())
            {
                break;
            }
            if (peek == '\n')
            {
                lexed_tokens.push_back(tokens::Newline{});
            }
            std::string token;
            is >> token;
            lexed_tokens.push_back(to_token(token));
        }
        return lexed_tokens;
    }

    void print_tokens(const std::vector<tokens::AnyToken> &tokens)
    {
        for (const auto &token : tokens)
        {
            std::visit(
                []<tokens::Token T>(const T &t)
                {
                    std::cout << t;
                    if constexpr (std::is_same_v<T, tokens::Newline>)
                    {
                        std::cout << "\n";
                    }
                    else
                    {
                        std::cout << " ";
                    }
                },
                token);
        }
    }
} // namespace lexer
