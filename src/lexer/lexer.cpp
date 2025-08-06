#include "lexer/lexer.hpp"
#include "lexer.hpp"
#include <cctype>
#include <utility>

Lexer::Lexer(const File &file, DiagnosticEngine &diag_engine)
    : file(file), cursor(0), diag_engine(diag_engine), tokens({}) {}

void Lexer::lex() {}

bool Lexer::is_at_end(const unsigned k) const {
  if (cursor + k >= file.length)
    return true;
  return false;
}

char Lexer::current() const {
  if (is_at_end())
    return '\0';
  return file.content.at(cursor);
}

char Lexer::peek(const unsigned k) {
  if (is_at_end(k))
    return '\0';
  return file.content.at(cursor);
}

char Lexer::advance(const unsigned k) {
  if (is_at_end(k))
    cursor = file.length;
  return current();
}

void Lexer::skip_whitespace() {
  while (std::isspace(peek()) && peek() != '\n')
    advance();
}

Token Lexer::advance_and_return(const Token::Kind kind, const Span span) {
  advance(span.length);
  return Token(kind, span);
}

std::optional<Token> Lexer::lex_operator() {
  using enum Token::Kind;

  const auto ch0 = current();
  const auto ch1 = peek();
  const auto ch2 = peek(2);
  const auto start = cursor;

  switch (ch0) {
  case '+': {
    if (ch1 == '+')
      return advance_and_return(PlusPlus, Span(file, start, 2));
    if (ch1 == '=')
      return advance_and_return(PlusEquals, Span(file, start, 2));
    return advance_and_return(Plus, Span(file, start, 1));
  }
  case '-': {
    if (ch1 == '-')
      return advance_and_return(MinusMinus, Span(file, start, 2));
    if (ch1 == '=')
      return advance_and_return(MinusEquals, Span(file, start, 2));
    if (ch1 == '>')
      return advance_and_return(Arrow, Span(file, start, 2));
    return advance_and_return(Minus, Span(file, start, 1));
  }
  case '*': {
    if (ch1 == '*' && ch2 == '=')
      return advance_and_return(StarStarEquals, Span(file, start, 3));
    if (ch1 == '*')
      return advance_and_return(StarStar, Span(file, start, 2));
    if (ch1 == '=')
      return advance_and_return(StarEquals, Span(file, start, 2));
    return advance_and_return(Star, Span(file, start, 1));
  }
  case '/': {
    if (ch1 == '/' && ch2 == '=')
      return advance_and_return(SlashSlashEquals, Span(file, start, 3));
    if (ch1 == '/')
      return advance_and_return(SlashSlash, Span(file, start, 2));
    if (ch1 == '=')
      return advance_and_return(SlashEquals, Span(file, start, 2));
    return advance_and_return(Slash, Span(file, start, 1));
  }
  case '=': {
    if (ch1 == '=')
      return advance_and_return(EqualsEquals, Span(file, start, 2));
    if (ch1 == '>')
      return advance_and_return(FatArrow, Span(file, start, 2));
    return advance_and_return(Equals, Span(file, start, 1));
  }
  case '!': {
    if (ch1 == '=')
      return advance_and_return(BangEquals, Span(file, start, 2));
    return advance_and_return(Bang, Span(file, start, 1));
  }
  case '<': {
    if (ch1 == '=')
      return advance_and_return(LessEquals, Span(file, start, 2));
    return advance_and_return(Less, Span(file, start, 1));
  }
  case '>': {
    if (ch1 == '=')
      return advance_and_return(MoreEquals, Span(file, start, 2));
    return advance_and_return(More, Span(file, start, 1));
  }
  case '&': {
    if (ch1 == '&')
      return advance_and_return(AndAnd, Span(file, start, 2));
    return advance_and_return(And, Span(file, start, 1));
  }
  case '|': {
    if (ch1 == '|')
      return advance_and_return(BarBar, Span(file, start, 2));
    return advance_and_return(Bar, Span(file, start, 1));
  }
  case '(':
    return advance_and_return(LParen, Span(file, start, 1));
  case ')':
    return advance_and_return(RParen, Span(file, start, 1));
  case '[':
    return advance_and_return(LBrac, Span(file, start, 1));
  case ']':
    return advance_and_return(RBrac, Span(file, start, 1));
  case '{':
    return advance_and_return(LCurl, Span(file, start, 1));
  case '}':
    return advance_and_return(RCurl, Span(file, start, 1));
  case '.':
    return advance_and_return(Dot, Span(file, start, 1));
  case ',':
    return advance_and_return(Comma, Span(file, start, 1));
  case ':':
    return advance_and_return(Colon, Span(file, start, 1));
  case ';':
    return advance_and_return(Semicolon, Span(file, start, 1));
  case '?':
    return advance_and_return(Question, Span(file, start, 1));
  case '%':
    return advance_and_return(Percent, Span(file, start, 1));
  default: {
  }
  }

  return std::nullopt;
}
