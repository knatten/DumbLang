#pragma once

#include <ostream>
#include <vector>

namespace lexer
{
    namespace tokens
    {
        struct Let
        {
            friend std::ostream &operator<<(std::ostream &os, const Let &let)
            {
                os << "Let";
                return os;
            }
        };
        struct Identifier
        {
            std::string value;
            friend std::ostream &operator<<(std::ostream &os,
                                            const Identifier &identifier)
            {
                os << "Identifier(" << identifier.value << ")";
                return os;
            }
        };
        struct Equals
        {
            friend std::ostream &operator<<(std::ostream &os,
                                            const Equals &equals)
            {
                os << "Equals";
                return os;
            }
        };
        struct Literal
        {
            int value;
            friend std::ostream &operator<<(std::ostream &os,
                                            const Literal &literal)
            {
                os << "Identifier(" << literal.value << ")";
                return os;
            }
        };
        struct Newline
        {
            friend std::ostream &operator<<(std::ostream &os,
                                            const Newline &equals)
            {
                os << "<CR>";
                return os;
            }
        };

        using token = std::variant<Let, Identifier, Equals, Literal, Newline>;
    } // namespace tokens
    tokens::token to_token(const std::string &s);
    std::vector<tokens::token> lex(std::istream &is);
    void print_tokens(const std::vector<tokens::token> &tokens);
}