#pragma once

#include "traits.h"

#include <ostream>
#include <vector>

namespace lexer
{
    namespace tokens
    {
        struct Let
        {
            friend std::ostream &operator<<(std::ostream &os, const Let &)
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
        struct Assignment
        {
            friend std::ostream &operator<<(std::ostream &os,
                                            const Assignment &)
            {
                os << "Assignment";
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
            friend std::ostream &operator<<(std::ostream &os, const Newline &)
            {
                os << "<CR>";
                return os;
            }
        };

        template <typename T>
        concept Token =
            is_any<T, Let, Identifier, Assignment, Literal, Newline>::value;

        using AnyToken =
            std::variant<Let, Identifier, Assignment, Literal, Newline>;

        namespace detail
        {
            template <typename T>
            concept HasValue = requires(T t) { t.value; };
        } // namespace detail

        template <typename T>
        concept TokenWithValue = Token<T> && detail::HasValue<T>;

        namespace detail
        {
            template <Token Tok1, Token Tok2>
            bool has_same_value_if_any(const Tok1 &, const Tok2 &)
            {
                return true;
            }
            template <TokenWithValue Tok1, TokenWithValue Tok2>
            bool has_same_value_if_any(const Tok1 &tok1, const Tok2 &tok2)
            {
                return tok1.value == tok2.value;
            }
        } // namespace detail

        template <Token Tok1, Token Tok2>
        bool operator==(const Tok1& tok1, const Tok2& tok2)
        {
            if constexpr (!std::is_same_v<Tok1, Tok2>)
            {
                return false;
            }
            return detail::has_same_value_if_any(tok1, tok2);
        }
    } // namespace tokens
    tokens::AnyToken to_token(const std::string &s);
    std::vector<tokens::AnyToken> lex(std::istream &is);
    void print_tokens(const std::vector<tokens::AnyToken> &tokens);
} // namespace lexer