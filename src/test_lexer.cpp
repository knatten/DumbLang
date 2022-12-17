#include "lexer.h"

#include "catch2/catch_all.hpp"

#include <sstream>

using Catch::Matchers::Equals;

using namespace lexer::tokens;

static_assert(Token<Let>);
static_assert(!Token<int>);
static_assert(TokenWithValue<Identifier>);
static_assert(!TokenWithValue<Let>);

TEST_CASE("to_token")
{
    REQUIRE(std::holds_alternative<Identifier>(lexer::to_token("x")));
    REQUIRE(std::holds_alternative<Identifier>(lexer::to_token("x2")));
    REQUIRE(std::holds_alternative<Identifier>(lexer::to_token("2x")));
    REQUIRE(std::holds_alternative<Literal>(lexer::to_token("2")));
    REQUIRE(std::holds_alternative<Literal>(lexer::to_token("2235")));
    REQUIRE(std::holds_alternative<Newline>(lexer::to_token("\n")));
    REQUIRE(std::holds_alternative<Let>(lexer::to_token("let")));
    REQUIRE(std::holds_alternative<Identifier>(lexer::to_token("lett")));
    REQUIRE(std::holds_alternative<Assignment>(lexer::to_token("=")));
}

TEST_CASE("Comparing tokens")
{
    REQUIRE(Let{} == Let{});
    REQUIRE(Let{} != Assignment{});
    REQUIRE(Identifier{"foo"} != Identifier{"bar"});
    REQUIRE(Identifier{"foo"} == Identifier{"foo"});
}

TEST_CASE("lex")
{
    {
        std::stringstream ss{"let x"};
        const auto tokens = lexer::lex(ss);
        REQUIRE_THAT(tokens,
                     Equals(std::vector<AnyToken>{Let{}, Identifier{"x"}}));
    }
    {
        std::stringstream ss{""};
        const auto tokens = lexer::lex(ss);
        REQUIRE_THAT(tokens, Equals(std::vector<AnyToken>{}));
    }
    {
        std::stringstream ss{"let\n2"};
        const auto tokens = lexer::lex(ss);
        REQUIRE_THAT(tokens, Equals(std::vector<AnyToken>{Let{}, Newline{},
                                                          Literal{2}}));
    }
}

template <Token T> std::string to_string(T t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

TEST_CASE("ostream")
{
    REQUIRE("Let" == to_string(Let{}));
    REQUIRE("Identifier(x)" == to_string(Identifier{"x"}));
    REQUIRE("Assignment" == to_string(Assignment{}));
    REQUIRE("Literal(2)" == to_string(Literal{2}));
    REQUIRE("<CR>" == to_string(Newline{}));
}
