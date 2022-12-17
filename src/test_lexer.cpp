#include "lexer.h"

#include "catch2/catch_all.hpp"

#include <sstream>

using Catch::Matchers::Equals;

using namespace lexer::tokens;

TEST_CASE("to_token")
{
    REQUIRE(std::holds_alternative<Identifier>(
        lexer::to_token("x")));
    REQUIRE(std::holds_alternative<Identifier>(
        lexer::to_token("x2")));
    REQUIRE(std::holds_alternative<Identifier>(
        lexer::to_token("2x"))); //Yes, actually!
    REQUIRE(std::holds_alternative<Literal>(
        lexer::to_token("2")));
    REQUIRE(std::holds_alternative<Literal>(
        lexer::to_token("2235")));
    REQUIRE(std::holds_alternative<Newline>(
        lexer::to_token("\n")));
    REQUIRE(std::holds_alternative<Let>(
        lexer::to_token("let")));
    REQUIRE(std::holds_alternative<Identifier>(
        lexer::to_token("lett")));
    REQUIRE(std::holds_alternative<Assignment>(
        lexer::to_token("=")));
}