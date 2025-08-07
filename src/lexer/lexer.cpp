#include "lexer/lexer.hpp"
#include "common/diagnostic.hpp"
#include <cctype>
#include <utility>

constexpr const char *ERROR_MSG_NEWLINE_IN_STRING =
    "Newlines are not allowed in string literals. Change this to a raw string "
    "literal with `\"\"\"` instead or remove the newline.";

constexpr const char *ERROR_MSG_MISSING_DELIMITER =
    "This string literal has no closing `\"`!";

bool is_identifier_start(const char &ch) {
  return std::isalpha(ch) || ch == '_';
}

bool is_identifier_cont(const char &ch) {
  return std::isalnum(ch) || ch == '_';
}

bool is_numeric_start(const char &ch) { return std::isdigit(ch); }
bool is_numeric_cont(const char &ch) { return std::isdigit(ch) || ch == '.'; }

bool is_str_literal_disallowed(const char &ch) {
  return (ch == '\n' || ch == '\r');
}

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

char Lexer::eat(const unsigned k) {
  if (is_at_end(k))
    cursor = file.length;
  return current();
}

void Lexer::skip_whitespace() {
  while (std::isspace(peek()) && peek() != '\n')
    eat();
}

Token Lexer::eat_and_emit(const Token::Kind kind, const Span span) {
  eat(span.length);
  return Token(kind, span);
}

Token Lexer::lex_identifier() {
  const auto start = cursor;
  while (is_identifier_cont(peek()))
    eat();

  // Once the above condition fails, create the token
  const auto span = Span(file, start, cursor - start + 1);
  const auto lexeme = std::string(span.get_lexeme());
  const auto kind = maybe_keyword(lexeme);

  eat(); // move past the last char
  return Token(kind, span);
}

Token Lexer::lex_numeric() {
  const auto start = cursor;
  auto kind = Token::Kind::Integer;

  while (is_numeric_cont(peek())) {
    if (peek() == '.') {
      // Check to see if the token isn't already a float and if the thing after
      // the dot is actually a number
      if (kind != Token::Kind::Float && is_numeric_start(peek(2))) {
        kind = Token::Kind::Float;
        eat(2);
        continue;
      }

      // Otherwise, this dot is its own token, so stop here
      break;
    }

    // If it isn't a dot but still numeric_cont, then advance and continue
    eat();
  }

  // Create the token
  const auto span = Span(file, start, cursor - start + 1);
  eat(); // move past the last char
  return Token(kind, span);
}

std::optional<Token> Lexer::lex_string() {
  const auto start = cursor;
  bool error_found = false;

  while (1) {
    if (is_str_literal_disallowed(peek())) {
      const auto diag_span = Span(file, cursor, 1);
      diag_engine.push(Diagnostic(diagnostic::Kind::InvalidString, diag_span,
                                  ERROR_MSG_NEWLINE_IN_STRING));
      error_found = true;
      // Skip to the closing delimiter
      while (current() != '"')
        eat();
    }

    // Check if the EOF was found and throw an error
    if (current() == '\0') {
      const auto diag_span = Span(file, cursor - 1, 1);
      diag_engine.push(Diagnostic(diagnostic::Kind::UnterminatedString,
                                  diag_span, ERROR_MSG_MISSING_DELIMITER));
      return std::nullopt;
    }

    // If closing delimiter found, eat it and break
    if (peek() == '"') {
      eat();
      break;
    }

    // Otherwise keep going
    eat();
  }

  if (error_found)
    return std::nullopt;

  // Create the token
  const auto span = Span(file, start, cursor - start + 1);
  eat(); // move past the last char
  return Token(Token::Kind::String, span);
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
      return eat_and_emit(PlusPlus, Span(file, start, 2));
    if (ch1 == '=')
      return eat_and_emit(PlusEquals, Span(file, start, 2));
    return eat_and_emit(Plus, Span(file, start, 1));
  }
  case '-': {
    if (ch1 == '-')
      return eat_and_emit(MinusMinus, Span(file, start, 2));
    if (ch1 == '=')
      return eat_and_emit(MinusEquals, Span(file, start, 2));
    if (ch1 == '>')
      return eat_and_emit(Arrow, Span(file, start, 2));
    return eat_and_emit(Minus, Span(file, start, 1));
  }
  case '*': {
    if (ch1 == '*' && ch2 == '=')
      return eat_and_emit(StarStarEquals, Span(file, start, 3));
    if (ch1 == '*')
      return eat_and_emit(StarStar, Span(file, start, 2));
    if (ch1 == '=')
      return eat_and_emit(StarEquals, Span(file, start, 2));
    return eat_and_emit(Star, Span(file, start, 1));
  }
  case '/': {
    if (ch1 == '/' && ch2 == '=')
      return eat_and_emit(SlashSlashEquals, Span(file, start, 3));
    if (ch1 == '/')
      return eat_and_emit(SlashSlash, Span(file, start, 2));
    if (ch1 == '=')
      return eat_and_emit(SlashEquals, Span(file, start, 2));
    return eat_and_emit(Slash, Span(file, start, 1));
  }
  case '=': {
    if (ch1 == '=')
      return eat_and_emit(EqualsEquals, Span(file, start, 2));
    if (ch1 == '>')
      return eat_and_emit(FatArrow, Span(file, start, 2));
    return eat_and_emit(Equals, Span(file, start, 1));
  }
  case '!': {
    if (ch1 == '=')
      return eat_and_emit(BangEquals, Span(file, start, 2));
    return eat_and_emit(Bang, Span(file, start, 1));
  }
  case '<': {
    if (ch1 == '=')
      return eat_and_emit(LessEquals, Span(file, start, 2));
    return eat_and_emit(Less, Span(file, start, 1));
  }
  case '>': {
    if (ch1 == '=')
      return eat_and_emit(MoreEquals, Span(file, start, 2));
    return eat_and_emit(More, Span(file, start, 1));
  }
  case '&': {
    if (ch1 == '&')
      return eat_and_emit(AndAnd, Span(file, start, 2));
    return eat_and_emit(And, Span(file, start, 1));
  }
  case '|': {
    if (ch1 == '|')
      return eat_and_emit(BarBar, Span(file, start, 2));
    return eat_and_emit(Bar, Span(file, start, 1));
  }
  case '(':
    return eat_and_emit(LParen, Span(file, start, 1));
  case ')':
    return eat_and_emit(RParen, Span(file, start, 1));
  case '[':
    return eat_and_emit(LBrac, Span(file, start, 1));
  case ']':
    return eat_and_emit(RBrac, Span(file, start, 1));
  case '{':
    return eat_and_emit(LCurl, Span(file, start, 1));
  case '}':
    return eat_and_emit(RCurl, Span(file, start, 1));
  case '.':
    return eat_and_emit(Dot, Span(file, start, 1));
  case ',':
    return eat_and_emit(Comma, Span(file, start, 1));
  case ':':
    return eat_and_emit(Colon, Span(file, start, 1));
  case ';':
    return eat_and_emit(Semicolon, Span(file, start, 1));
  case '?':
    return eat_and_emit(Question, Span(file, start, 1));
  case '%':
    return eat_and_emit(Percent, Span(file, start, 1));
  default:
    return std::nullopt;
  }
}
