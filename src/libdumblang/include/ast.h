#pragma once

#include <memory>
#include <string>

namespace AST
{
    struct Expression
    {
        virtual ~Expression() {}
    };

    struct Identifier : Expression
    {
        Identifier(std::string name_) : name(std::move(name_)) {}
        std::string name;
    };

    struct Literal : Expression
    {
        Literal(int value_) : value(value_) {}
        int value;
    };

    struct Assignment : Expression
    {
        Assignment(std::unique_ptr<Identifier> &&lhs_,
                   std::unique_ptr<Expression> &&rhs_)
            : lhs{std::move(lhs_)}, rhs{std::move(rhs_)}
        {
        }
        std::unique_ptr<Identifier> lhs;
        std::unique_ptr<Expression> rhs;
    };
} // namespace AST
