#include "ast_dumper.h"

#include <fmt/format.h>
#include <ostream>

namespace AST
{
    void AstDumper::print(std::string_view str)
    {
        const auto indentation = fmt::format("{:{}}", "", m_indentation * 2);
        m_os << fmt::format("{}{}\n", indentation, str);
    }

    template <typename Info>
    void AstDumper::print(std::string_view expr_name, const Info &info)
    {
        print(fmt::format("{}: '{}'", expr_name, info));
    }

    void AstDumper::dump()
    {
        for (const auto &expr : m_program)
        {
            expr->accept(*this);
        }
    }

    void AstDumper::visit(const Expression &) { m_os << "UNKNOWN_EXPRESSION"; }

    void AstDumper::visit(const Identifier &identifier)
    {
        print("IDENTIFIER", identifier.name);
    }

    void AstDumper::visit(const Literal &literal)
    {
        print("LITERAL", literal.value);
    }

    void AstDumper::visit(const Assignment &assignment)
    {
        print("ASSIGNMENT");
        Indenter indenter(m_indentation);
        assignment.lhs->accept(*this);
        assignment.rhs->accept(*this);
    }
} // namespace AST
