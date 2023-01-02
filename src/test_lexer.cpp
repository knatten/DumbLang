#include "lexer.h"

#include <catch2/catch.hpp>


#include <sstream>

using Catch::Matchers::Equals;

using namespace lexer::tokens;

static_assert(Token<Let>);
static_assert(!Token<int>);
static_assert(TokenWithValue<Identifier>);
static_assert(!TokenWithValue<Let>);

TEST_CASE("to_token")
{
    CHECK(std::holds_alternative<Identifier>(lexer::to_token("x")));
    CHECK(std::holds_alternative<Identifier>(lexer::to_token("x2")));
    CHECK(std::holds_alternative<Identifier>(lexer::to_token("2x")));
    CHECK(std::holds_alternative<Literal>(lexer::to_token("2")));
    CHECK(std::holds_alternative<Literal>(lexer::to_token("2235")));
    CHECK(std::holds_alternative<Newline>(lexer::to_token("\n")));
    CHECK(std::holds_alternative<Let>(lexer::to_token("let")));
    CHECK(std::holds_alternative<Identifier>(lexer::to_token("lett")));
    CHECK(std::holds_alternative<Assignment>(lexer::to_token("=")));
}

TEST_CASE("Comparing tokens")
{
    CHECK(Let{} == Let{});
    CHECK(Let{} != Assignment{});
    CHECK(Identifier{"foo"} != Identifier{"bar"});
    CHECK(Identifier{"foo"} == Identifier{"foo"});
}

TEST_CASE("lex")
{
    {
        std::stringstream ss{"let x"};
        const auto tokens = lexer::lex(ss);
        CHECK_THAT(tokens,
                     Equals(std::vector<AnyToken>{Let{}, Identifier{"x"}}));
    }
    {
        std::stringstream ss{""};
        const auto tokens = lexer::lex(ss);
        CHECK_THAT(tokens, Equals(std::vector<AnyToken>{}));
    }
    {
        std::stringstream ss{"let\n2"};
        const auto tokens = lexer::lex(ss);
        CHECK_THAT(tokens, Equals(std::vector<AnyToken>{Let{}, Newline{},
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
    CHECK("Let" == to_string(Let{}));
    CHECK("Identifier(x)" == to_string(Identifier{"x"}));
    CHECK("Assignment" == to_string(Assignment{}));
    CHECK("Literal(2)" == to_string(Literal{2}));
    CHECK("<CR>" == to_string(Newline{}));
}
