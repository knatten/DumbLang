#include "ast.h"
#include "lexer.h"
#include "parser.h"

#include <catch2/catch.hpp>

#include <memory>
#include <span>
#include <sstream>
#include <string>

namespace ts = lexer::tokens;

template <typename T>
const T &getExpression(const parser::ParseResult<T> &result)
{
    const auto &up = std::get<std::unique_ptr<T>>(result);
    if (!up)
    {
        throw std::runtime_error("Expression is null");
    }
    return *up;
}

template <typename T>
const T &
getDowncastExpression(const parser::ParseResult<AST::Expression> &parseResult)
{
    const AST::Expression &expression = getExpression(parseResult);
    const T *const downCastExpression =
        dynamic_cast<const T *const>(&expression);
    if (!downCastExpression)
    {
        throw std::runtime_error("Expression is not of correct type");
    }
    return *downCastExpression;
}

TEST_CASE("Parse single tokens")
{
    SECTION("Successful parse of identifier")
    {
        std::vector<ts::AnyToken> tokens{ts::Identifier{"x"}, ts::Literal{2}};
        const auto parseResult = parser::parseIdentifier(tokens);
        REQUIRE(std::get<parser::TokenSpan>(parseResult).size() == 1);
        REQUIRE(getExpression(parseResult).name == "x");
    }

    SECTION("Successful parse of literal")
    {
        std::vector<ts::AnyToken> tokens{ts::Literal{2}, ts::Identifier{"2"}};
        const auto parseResult = parser::parseLiteral(tokens);
        REQUIRE(std::get<parser::TokenSpan>(parseResult).size() == 1);
        REQUIRE(getExpression(parseResult).value == 2);
    }

    SECTION("Incorrect type")
    {
        std::vector<ts::AnyToken> tokens{ts::Literal{2}, ts::Identifier{"x"}};
        const auto parseResult = parser::parseIdentifier(tokens);
        REQUIRE(std::get<parser::TokenSpan>(parseResult).size() == 2);
        REQUIRE(std::get<std::unique_ptr<AST::Identifier>>(parseResult) ==
                nullptr);
    }

    SECTION("Empty span")
    {
        std::vector<ts::AnyToken> tokens{};
        const auto parseResult = parser::parseLiteral(tokens);
        REQUIRE(std::get<parser::TokenSpan>(parseResult).size() == 0);
        REQUIRE(std::get<std::unique_ptr<AST::Literal>>(parseResult) ==
                nullptr);
    }
}

TEST_CASE("Parse assignment")
{
    SECTION("Assign from literal")
    {
        std::vector<ts::AnyToken> tokens{ts::Let{}, ts::Identifier{"x"},
                                         ts::Assignment{}, ts::Literal{2}};
        AST::Assignment expected{std::make_unique<AST::Identifier>("x"),
                                 std::make_unique<AST::Literal>(2)};
        const auto parseResult = parser::parseAssignment(tokens);
        REQUIRE(getExpression(parseResult) == expected);
        REQUIRE(std::get<parser::TokenSpan>(parseResult).size() == 0);
    }
    SECTION("Assign from identifier")
    {
        std::vector<ts::AnyToken> tokens{ts::Let{}, ts::Identifier{"x"},
                                         ts::Assignment{}, ts::Identifier{"y"}};
        AST::Assignment expected{std::make_unique<AST::Identifier>("x"),
                                 std::make_unique<AST::Identifier>("y")};
        const auto parseResult = parser::parseAssignment(tokens);
        REQUIRE(getExpression(parseResult) == expected);
        REQUIRE(std::get<parser::TokenSpan>(parseResult).size() == 0);
    }
    SECTION("Not assignment")
    {
        std::vector<ts::AnyToken> tokens{ts::Literal{2}, ts::Identifier{"2"}};
        const auto parseResult = parser::parseAssignment(tokens);
        REQUIRE(std::get<parser::TokenSpan>(parseResult).size() ==
                tokens.size());
        REQUIRE(std::get<std::unique_ptr<AST::Assignment>>(parseResult) ==
                nullptr);
        tokens.clear();
        const auto parseResult2 = parser::parseAssignment(tokens);
        REQUIRE(std::get<parser::TokenSpan>(parseResult2).size() ==
                tokens.size());
        REQUIRE(std::get<std::unique_ptr<AST::Assignment>>(parseResult2) ==
                nullptr);
    }
    SECTION("Malformed assignment")
    {
        std::vector<ts::AnyToken> tokens{ts::Let{}, ts::Identifier{"x"}};
        parser::parseAssignment(tokens);
        // TODO how to do error handling?
    }
}

TEST_CASE("Parse expression")
{
    SECTION("Parse literal")
    {
        std::vector<ts::AnyToken> tokens{ts::Literal{2}, ts::Identifier{"2"}};
        const auto parseResult = parser::parseExpression(tokens);
        REQUIRE(std::get<parser::TokenSpan>(parseResult).size() == 1);
        REQUIRE(getDowncastExpression<AST::Literal>(parseResult) ==
                AST::Literal{2});
    }
    SECTION("Parse identifier")
    {
        std::vector<ts::AnyToken> tokens{ts::Identifier{"x"}, ts::Literal{2}};
        const auto parseResult = parser::parseExpression(tokens);
        REQUIRE(std::get<parser::TokenSpan>(parseResult).size() == 1);
        REQUIRE(getDowncastExpression<AST::Identifier>(parseResult) ==
                AST::Identifier{"x"});
    }
    SECTION("Parse assignment")
    {
        std::vector<ts::AnyToken> tokens{ts::Let{}, ts::Identifier{"x"},
                                         ts::Assignment{}, ts::Identifier{"y"},
                                         ts::Let{}};
        const auto parseResult = parser::parseExpression(tokens);
        REQUIRE(std::get<parser::TokenSpan>(parseResult).size() == 1);
        AST::Assignment expected{std::make_unique<AST::Identifier>("x"),
                                 std::make_unique<AST::Identifier>("y")};
        REQUIRE(getDowncastExpression<AST::Assignment>(parseResult) ==
                expected);
    }
}

AST::Program parse(std::string s)
{
    std::stringstream ss(std::move(s));
    return parser::parse(ss);
}

TEST_CASE("Parse full program")
{
    SECTION("Parse empty program")
    {
        {
            const auto program = parse("");
            REQUIRE(program.expressions.size() == 0);
        }
        {
            const auto program = parse("\n");
            REQUIRE(program.expressions.size() == 0);
        }
    }
    SECTION("Parse single line")
    {
        const auto program = parse("let x = 42");
        REQUIRE(program.expressions.size() == 1);
        AST::Assignment expected{std::make_unique<AST::Identifier>("x"),
                                 std::make_unique<AST::Literal>(42)};
        REQUIRE(program.expressions.size() == 1);
        REQUIRE(*(program.expressions[0]) == expected);
    }

    SECTION("Parse multiple lines")
    {
        const auto program = parse("let x = 42\nx");
        REQUIRE(program.expressions.size() == 2);
        AST::Assignment expected1{std::make_unique<AST::Identifier>("x"),
                                  std::make_unique<AST::Literal>(42)};
        AST::Identifier expected2{"x"};
        REQUIRE(program.expressions.size() == 2);
        REQUIRE(*(program.expressions[0]) == expected1);
        REQUIRE(*(program.expressions[1]) == expected2);
    }

    // TODO error handling. Expression with trailing tokens on same line,
    // incomplete expression
}