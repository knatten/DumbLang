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
