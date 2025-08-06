#ifndef LEXER_H
#define LEXER_H
#include "common/diagnostic.hpp"
#include "common/span.hpp"
#include "lexer/token.hpp"
#include <optional>

class Lexer {
  const File &file;
  size_t cursor;

  DiagnosticEngine &diag_engine;
  std::vector<Token> tokens;

public:
  Lexer(const File &file, DiagnosticEngine &diag_engine);
  void lex();

private:
  bool is_at_end(const unsigned k = 0) const;
  char current() const;
  char peek(const unsigned k = 1);
  char advance(const unsigned k = 0);
  void skip_whitespace();

  Token advance_and_return(const Token::Kind kind, const Span span);

  std::optional<Token> lex_identifier();
  std::optional<Token> lex_numeric();
  std::optional<Token> lex_string();
  std::optional<Token> lex_operator();
  std::optional<Token> lex_once();
};

#endif