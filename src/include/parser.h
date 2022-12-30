#pragma once

#include "ast.h"
#include "lexer.h"

#include <span>

namespace parser
{
    using TokenSpan = std::span<lexer::tokens::AnyToken>;
    template <typename Expression>
    using ParseResult = std::tuple<TokenSpan, std::unique_ptr<Expression>>;

    ParseResult<AST::Identifier> parseIdentifier(TokenSpan tokens);
    ParseResult<AST::Literal> parseLiteral(TokenSpan tokens);
    ParseResult<AST::Assignment> parseAssignment(TokenSpan tokens);
    ParseResult<AST::Expression> parseExpression(TokenSpan tokens);
} // namespace parser