#include "ast.h"

namespace AST
{
    bool Program::operator==(const Program &other) const
    {
        if (expressions.size() != other.expressions.size())
        {
            return false;
        }
        for (size_t i = 0; i < expressions.size(); ++i)
        {
            if (*(expressions[i]) != *(other.expressions[i]))
            {
                return false;
            }
        }
        return true;
    }
} // namespace AST