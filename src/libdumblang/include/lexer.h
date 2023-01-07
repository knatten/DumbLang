#pragma once

#include "traits.h"

#include <ostream>
#include <variant>
#include <vector>

namespace lexer
{
    namespace tokens
    {
        struct Let
        {
            static inline std::string display_name{"Let"};
        };

        struct Identifier
        {
            static inline std::string display_name{"Identifier"};
            std::string value;
        };

        struct Assignment
        {
            static inline std::string display_name{"Assignment"};
        };

        struct Literal
        {
            static inline std::string display_name{"Literal"};
            int value;
        };

        struct Newline
        {
            static inline std::string display_name{"<CR>"};
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
        bool operator==(const Tok1 &tok1, const Tok2 &tok2)
        {
            if constexpr (!std::is_same_v<Tok1, Tok2>)
            {
                return false;
            }
            return detail::has_same_value_if_any(tok1, tok2);
        }

        template <Token T> std::ostream &operator<<(std::ostream &os, const T &)
        {
            os << T::display_name;
            return os;
        }
        template <TokenWithValue T>
        std::ostream &operator<<(std::ostream &os, const T &token)
        {
            os << T::display_name << "(" << token.value << ")";
            return os;
        }
    } // namespace tokens
    tokens::AnyToken to_token(const std::string &s);
    std::vector<tokens::AnyToken> lex(std::istream &is);
    void print_tokens(const std::vector<tokens::AnyToken> &tokens);
} // namespace lexer