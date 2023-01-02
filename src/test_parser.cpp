#include "ast.h"
#include "lexer.h"
#include "parser.h"

#include <catch2/catch.hpp>

#include <memory>
#include <span>
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

template <typename To, typename From>
const To &downcast(const std::unique_ptr<From> &from)
{
    const To *to = dynamic_cast<const To *>(from.get());
    if (!to)
    {
        throw std::runtime_error("Could not downcast");
    }
    return *to;
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
        const auto parseResult = parser::parseAssignment(tokens);
        REQUIRE(std::get<parser::TokenSpan>(parseResult).size() == 0);
        const auto &assignment = getExpression(parseResult);
        REQUIRE(assignment.lhs->name == "x");
        const auto &rhs = downcast<AST::Literal>(assignment.rhs);
        REQUIRE(rhs.value == 2);
    }
    SECTION("Assign from identifier")
    {
        std::vector<ts::AnyToken> tokens{ts::Let{}, ts::Identifier{"x"},
                                         ts::Assignment{}, ts::Identifier{"y"}};
        const auto parseResult = parser::parseAssignment(tokens);
        REQUIRE(std::get<parser::TokenSpan>(parseResult).size() == 0);
        const auto &assignment = getExpression(parseResult);
        REQUIRE(assignment.lhs->name == "x");
        const auto &rhs = downcast<AST::Identifier>(assignment.rhs);
        REQUIRE(rhs.name == "y");
    }
    SECTION("Not assignment")
    {
        std::vector<ts::AnyToken> tokens{ts::Literal{2}, ts::Identifier{"2"}};
        const auto parseResult = parser::parseAssignment(tokens);
        REQUIRE(std::get<parser::TokenSpan>(parseResult).size() ==
                tokens.size());
        tokens.clear();
        const auto parseResult2 = parser::parseAssignment(tokens);
        REQUIRE(std::get<parser::TokenSpan>(parseResult2).size() ==
                tokens.size());
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
        REQUIRE(getDowncastExpression<AST::Literal>(parseResult).value == 2);
    }
    SECTION("Parse identifier")
    {
        std::vector<ts::AnyToken> tokens{ts::Identifier{"x"}, ts::Literal{2}};
        const auto parseResult = parser::parseExpression(tokens);
        REQUIRE(std::get<parser::TokenSpan>(parseResult).size() == 1);
        REQUIRE(getDowncastExpression<AST::Identifier>(parseResult).name ==
                "x");
    }
    SECTION("Parse assignment")
    {
        std::vector<ts::AnyToken> tokens{ts::Let{}, ts::Identifier{"x"},
                                         ts::Assignment{}, ts::Identifier{"y"},
                                         ts::Let{}};
        const auto parseResult = parser::parseExpression(tokens);
        REQUIRE(std::get<parser::TokenSpan>(parseResult).size() == 1);
        REQUIRE_NOTHROW(getDowncastExpression<AST::Assignment>(parseResult));
    }
}