#include "ast.h"
#include "parser.h"

#include <catch2/catch.hpp>

TEST_CASE("Comparing AST nodes")
{
    AST::Literal l1{1};
    AST::Literal l2{2};
    AST::Literal l3{1};
    REQUIRE(l1 == l1);
    REQUIRE(l1 != l2);
    REQUIRE(l1 == l1);

    AST::Identifier i1{"x"};
    AST::Identifier i2{"y"};
    AST::Identifier i3{"y"};
    REQUIRE(i1 == i1);
    REQUIRE(i1 != i2);
    REQUIRE(i1 == i1);

    AST::Assignment a1{std::make_unique<AST::Identifier>("x"),
                       std::make_unique<AST::Literal>(1)};
    AST::Assignment a2{std::make_unique<AST::Identifier>("x"),
                       std::make_unique<AST::Literal>(2)};
    AST::Assignment a3{std::make_unique<AST::Identifier>("x"),
                       std::make_unique<AST::Literal>(1)};
    AST::Assignment a4{std::make_unique<AST::Identifier>("y"),
                       std::make_unique<AST::Identifier>("x")};
    AST::Assignment a5{std::make_unique<AST::Identifier>("y"),
                       std::make_unique<AST::Identifier>("x")};
    REQUIRE(a1 == a1);
    REQUIRE(a1 != a2);
    REQUIRE(a1 == a3);
    REQUIRE(a1 != a4);
    REQUIRE(a4 == a5);

    // Different types
    REQUIRE(l1 != i1);
    REQUIRE(i1 != l1);
    REQUIRE(i1 != a1);
    REQUIRE(a1 != l1);
}

TEST_CASE("Comparing programs")
{
    //Cheat and use parser here since it's cumbersome to create vector of unique_ptr
    //TODO maybe instead make a better factory function or constructor. But how to do that for nested expressions?
    AST::Program p1 = parser::parse("let x = 42\n let y = x");
    AST::Program p2 = parser::parse("let x = 40\n let y = x");
    AST::Program p3 = parser::parse("let x = 42\n let y = x");
    REQUIRE(p1 == p1);
    REQUIRE(p1 != p2);
    REQUIRE(p1 == p3);
}