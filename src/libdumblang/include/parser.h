#pragma once

#include "ast.h"
#include "lexer.h"

#include <span>
#include <vector>

namespace parser
{
    using TokenSpan = std::span<lexer::tokens::AnyToken>;
    template <typename Expression>
    using ParseResult = std::tuple<TokenSpan, std::unique_ptr<Expression>>;

    AST::Program parse(TokenSpan tokens);
    AST::Program parse(std::istream &is);
    AST::Program parse(std::string s);

    ParseResult<AST::Identifier> parseIdentifier(TokenSpan tokens);
    ParseResult<AST::Identifier> parseIdentifier(TokenSpan tokens);
    ParseResult<AST::Literal> parseLiteral(TokenSpan tokens);
    ParseResult<AST::Assignment> parseAssignment(TokenSpan tokens);
    ParseResult<AST::Expression> parseExpression(TokenSpan tokens);
} // namespace parser