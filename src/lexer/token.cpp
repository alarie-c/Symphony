#include "lexer/token.hpp"

Token::Token(Kind kind, Span span) : kind(kind), span(span) {}

std::ostream &operator<<(std::ostream &os, const Token &token) {
  auto repr = get_token_repr(token.kind);
  auto span = token.span;
  os << span << " : " << repr;
  return os;
}
