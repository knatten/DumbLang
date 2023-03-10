#include "parser.h"
#include "ast.h"

namespace ts = lexer::tokens;

namespace
{

    struct NullExpression : AST::Expression
    {
        void accept(AST::Visitor &visitor) const override
        {
            visitor.visit(*this);
        }
        bool equals(const AST::Expression &) const override { return true; }
    };
    template <typename T> T *get_token_if(parser::TokenSpan tokens)
    {
        if (tokens.empty())
            return nullptr;
        return std::get_if<T>(&(tokens[0]));
    }

    template <typename T>
    parser::ParseResult<NullExpression> skipToken(parser::TokenSpan tokens)
    {
        if (get_token_if<T>(tokens))
        {
            return {tokens.subspan(1), std::make_unique<NullExpression>()};
        }
        return {tokens, nullptr};
    }

    template <typename TokenT, typename ASTT>
    parser::ParseResult<ASTT> parseSingleToken(parser::TokenSpan tokens)
    {
        if (const auto tok = get_token_if<TokenT>(tokens))
        {
            return {tokens.subspan(1), std::make_unique<ASTT>(tok->value)};
        }
        return {tokens, nullptr};
    }
} // namespace

namespace parser
{
    ParseResult<AST::Identifier> parseIdentifier(TokenSpan tokens)
    {
        return parseSingleToken<ts::Identifier, AST::Identifier>(tokens);
    }

    ParseResult<AST::Literal> parseLiteral(TokenSpan tokens)
    {
        return parseSingleToken<ts::Literal, AST::Literal>(tokens);
    }

    ParseResult<AST::Assignment> parseAssignment(TokenSpan tokens)
    {
        std::unique_ptr<NullExpression> nullExpression;
        std::tie(tokens, nullExpression) = skipToken<ts::Let>(tokens);
        if (!nullExpression)
        {
            return {tokens, nullptr};
        }
        std::unique_ptr<AST::Identifier> lhs;
        std::unique_ptr<AST::Expression> rhs;
        std::tie(tokens, lhs) = parseIdentifier(tokens);
        if (!lhs)
        {
            return {tokens, nullptr};
        }
        std::tie(tokens, nullExpression) = skipToken<ts::Assignment>(tokens);
        if (!nullExpression)
        {
            return {tokens, nullptr};
        }
        std::tie(tokens, rhs) = parseExpression(tokens);
        if (!rhs)
        {
            return {tokens, nullptr};
        }
        return {tokens, std::make_unique<AST::Assignment>(std::move(lhs),
                                                          std::move(rhs))};
    }

    ParseResult<AST::Expression> parseExpression(TokenSpan tokens)
    {
        std::unique_ptr<AST::Expression> expression;
        std::tie(tokens, expression) = parseAssignment(tokens);
        if (expression)
        {
            return {tokens, std::move(expression)};
        }
        std::tie(tokens, expression) = parseLiteral(tokens);
        if (expression)
        {
            return {tokens, std::move(expression)};
        }
        std::tie(tokens, expression) = parseIdentifier(tokens);
        if (expression)
        {
            return {tokens, std::move(expression)};
        }
        return {tokens, nullptr};
    }

    AST::Program parse(TokenSpan tokens)
    {
        AST::Program program;
        while (!tokens.empty())
        {
            if (std::holds_alternative<ts::Newline>(tokens[0]))
            {
                tokens = tokens.subspan(1);
                continue;
            }
            std::unique_ptr<AST::Expression> expression;
            std::tie(tokens, expression) = parseExpression(tokens);
            if (expression)
            {
                program.expressions.push_back(std::move(expression));
            }
            else
            {
                throw std::runtime_error("parse error");
            }
        }
        return program;
    }

    AST::Program parse(std::istream &is)
    {
        auto tokens = lexer::lex(is);
        return parse(tokens);
    }
} // namespace parser
