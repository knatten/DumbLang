#pragma once

#include <memory>
#include <string>
#include <vector>

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
        virtual bool equals(const Expression &other) const = 0;
        bool operator==(const Expression &other) const
        {
            return typeid(*this) == typeid(other) && equals(other);
        }
    };

    struct Identifier : Expression
    {
        Identifier(std::string name_) : name(std::move(name_)) {}
        void accept(Visitor &visitor) const override { visitor.visit(*this); }
        bool equals(const Expression &other) const override
        {
            return name == dynamic_cast<const Identifier &>(other).name;
        }
        std::string name;
    };

    struct Literal : Expression
    {
        Literal(int value_) : value(value_) {}
        void accept(Visitor &visitor) const override { visitor.visit(*this); }
        bool equals(const Expression &other) const override
        {
            return value == dynamic_cast<const Literal &>(other).value;
        }
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
        bool equals(const Expression &other) const override
        {
            const auto &otherAssignment =
                dynamic_cast<const Assignment &>(other);
            return *lhs == *(otherAssignment.lhs) &&
                   *rhs == *(otherAssignment.rhs);
        }
        std::unique_ptr<Identifier> lhs;
        std::unique_ptr<Expression> rhs;
    };

    struct Program
    {
        std::vector<std::unique_ptr<Expression>> expressions;
    };
} // namespace AST
