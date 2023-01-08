#pragma once

#include <memory>
#include <string>

namespace AST
{
    struct Expression;
    struct Identifier;
    struct Literal;
    struct Assignment;
    struct Visitor
    {
        virtual void
        visit(const Expression &) = 0; // Catch-all for unknown AST types
        virtual void visit(const Identifier &) = 0;
        virtual void visit(const Literal &) = 0;
        virtual void visit(const Assignment &) = 0;
        virtual ~Visitor() {}
    };

    struct Expression
    {
        virtual ~Expression() {}
        virtual void accept(Visitor &visitor) const = 0;
    };

    struct Identifier : Expression
    {
        Identifier(std::string name_) : name(std::move(name_)) {}
        void accept(Visitor &visitor) const override { visitor.visit(*this); }
        std::string name;
    };

    struct Literal : Expression
    {
        Literal(int value_) : value(value_) {}
        void accept(Visitor &visitor) const override { visitor.visit(*this); }
        int value;
    };

    struct Assignment : Expression
    {
        Assignment(std::unique_ptr<Identifier> &&lhs_,
                   std::unique_ptr<Expression> &&rhs_)
            : lhs{std::move(lhs_)}, rhs{std::move(rhs_)}
        {
        }
        void accept(Visitor &visitor) const override { visitor.visit(*this); }
        std::unique_ptr<Identifier> lhs;
        std::unique_ptr<Expression> rhs;
    };
} // namespace AST
