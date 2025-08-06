#ifndef TOKEN_H
#define TOKEN_H
#include "common/span.hpp"
#include <array>
#include <iostream>

#define TOKEN_LIST                                                             \
  X(LParen, "(")                                                               \
  X(RParen, ")")                                                               \
  X(LCurl, "{")                                                                \
  X(RCurl, "}")                                                                \
  X(LBrac, "[")                                                                \
  X(RBrac, "]")                                                                \
                                                                               \
  X(Plus, "+")                                                                 \
  X(PlusPlus, "++")                                                            \
  X(PlusEquals, "+=")                                                          \
  X(Minus, "-")                                                                \
  X(MinusMinus, "--")                                                          \
  X(MinusEquals, "-=")                                                         \
  X(Star, "*")                                                                 \
  X(StarEquals, "*=")                                                          \
  X(StarStar, "**")                                                            \
  X(StarStarEquals, "**-")                                                     \
  X(Slash, "/")                                                                \
  X(SlashEquals, "/=")                                                         \
  X(SlashSlash, "//")                                                          \
  X(SlashSlashEquals, "//=")                                                   \
                                                                               \
  X(Bang, "~")                                                                 \
  X(BangEquals, "!=")                                                          \
  X(Equals, "=")                                                               \
  X(EqualsEquals, "==")                                                        \
  X(Less, "<")                                                                 \
  X(LessEquals, "<=")                                                          \
  X(More, ">")                                                                 \
  X(MoreEquals, ">=")                                                          \
  X(And, "&")                                                                  \
  X(AndAnd, "&&")                                                              \
  X(Bar, "|")                                                                  \
  X(BarBar, "||")                                                              \
                                                                               \
  X(If, "if")                                                                  \
  X(Else, "else")                                                              \
  X(Return, "return")                                                          \
  X(For, "for")                                                                \
  X(While, "while")                                                            \
  X(In, "in")                                                                  \
  X(As, "as")                                                                  \
  X(Class, "class")                                                            \
  X(Enum, "enum")                                                              \
                                                                               \
  X(String, "string literal")                                                  \
  X(Identifier, "identifier literal")                                          \
  X(Integer, "integer literal")                                                \
  X(Float, "floating point literal")                                           \
  X(Boolean, "boolean literal")                                                \
                                                                               \
  X(Percent, "%")                                                              \
  X(Dot, ".")                                                                  \
  X(Comma, ",")                                                                \
  X(Arrow, "->")                                                               \
  X(FatArrow, "=>")                                                            \
  X(Colon, ":")                                                                \
  X(Semicolon, ";")                                                            \
  X(Newline, "<newline>")                                                      \
  X(Eof, "<eof>")

struct Token {
  enum class Kind {
#define X(name, repr) name,
    TOKEN_LIST
#undef X
  };

  const Kind kind;
  const Span span;

  Token(Kind kind, Span span);
  friend std::ostream &operator<<(std::ostream &os, const Token &token);
};

/// @brief Returns the textual representation of every token in ASCII
/// characters.
/// @param kind the kind to get repr for
/// @return a `const char *` representation of the token variant
constexpr const char *get_token_repr(const Token::Kind &kind) {
  switch (kind) {
#define X(name, repr)                                                          \
  case Token::Kind::name:                                                      \
    return repr;
    TOKEN_LIST
#undef X
  }
  return "<unknown>";
}

// For use in `TOKEN_OPERATOR_REPR_MAP` only...
#define REPR(tk) {Token::Kind::tk, get_token_repr(Token::Kind::tk)}

/// @brief An ordered array of every operator `Token::Kind` and it's
/// corresponding string representation.
constexpr std::array<std::pair<Token::Kind, const char *>, 39>
    TOKEN_OPERATOR_REPR_MAP = {{
        REPR(StarStarEquals),
        REPR(SlashSlashEquals),
        REPR(StarStar),
        REPR(StarEquals),
        REPR(SlashSlash),
        REPR(SlashEquals),
        REPR(PlusPlus),
        REPR(PlusEquals),
        REPR(MinusMinus),
        REPR(MinusEquals),
        REPR(BangEquals),
        REPR(EqualsEquals),
        REPR(LessEquals),
        REPR(MoreEquals),
        REPR(AndAnd),
        REPR(BarBar),
        REPR(Arrow),
        REPR(FatArrow),
        REPR(Star),
        REPR(Slash),
        REPR(Plus),
        REPR(Minus),
        REPR(Percent),
        REPR(Dot),
        REPR(Comma),
        REPR(Semicolon),
        REPR(Colon),
        REPR(Bar),
        REPR(And),
        REPR(Bang),
        REPR(Equals),
        REPR(Less),
        REPR(More),
        REPR(LParen),
        REPR(RParen),
        REPR(LBrac),
        REPR(RBrac),
        REPR(LCurl),
        REPR(RCurl),
    }};
#undef REPR

// For use in `maybe_keyword()` only...
#define MATCH(str, tk)                                                         \
  if (str == get_token_repr(Token::Kind::tk))                                  \
    return Token::Kind::tk;

/// @brief Returns a `Token::Kind` that corresponds with the string passed to
/// function.
/// @param str the string to match on
/// @return corresponding `Token::Kind` or just `Token::Kind::Identifier` if
/// nothing matches.
constexpr Token::Kind maybe_keyword(const std::string &str) {
  MATCH(str, If)
  MATCH(str, Else)
  MATCH(str, Return)
  MATCH(str, For)
  MATCH(str, While)
  MATCH(str, In)
  MATCH(str, As)
  MATCH(str, Class)
  MATCH(str, Enum)

  // If nothing else matches...
  return Token::Kind::Identifier;
}

#undef MATCH
#endif