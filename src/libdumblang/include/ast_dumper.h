#include "ast.h"

namespace AST
{
    class AstDumper : Visitor
    {
      public:
        AstDumper(const Program &program, std::ostream &os)
            : m_program(program), m_os(os)
        {
        }
        void dump();
        void visit(const Expression &) override;
        void visit(const Identifier &) override;
        void visit(const Literal &) override;
        void visit(const Assignment &) override;

      private:
        void print(std::string_view str);
        template <typename Info>
        void print(std::string_view expr_name, const Info &info);

        const Program &m_program;
        std::ostream &m_os;
        size_t m_indentation{0};

        class Indenter
        {
          public:
            Indenter(size_t &indentation) : m_indentation(indentation)
            {
                m_indentation++;
            }
            ~Indenter() { m_indentation--; }
            size_t &m_indentation;
        };
    };
} // namespace AST