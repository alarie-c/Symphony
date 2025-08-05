#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "common/diagnostic.hpp"
#include "common/span.hpp"

#include <string>

TEST_CASE("Testing code spans") {
  std::string source = "balls, world!";
  std::string path = "test.symph";
  auto const file = File(source, path);

  {
    auto span = Span(file, 0, 5);
    auto lexeme = span.get_lexeme();
    CHECK(lexeme == "balls");
  }
}

TEST_CASE("Testing file objects") {
  std::string source = "Symphony";
  std::string path = "test.symph";
  auto const file = File(source, path);

  CHECK(file.length == 8);
}

TEST_CASE("Single-character span produces correct caret line") {
  std::string source = "let x = 42;\nlet y = 10;";
  std::string path = "test.symph";
  File file(source, path);

  Span span(file, 4, 1);
  Diagnostic diag(diagnostic::Kind::UnexpectToken, span, "Unexpected token");

  std::string output = diag.print();

  CHECK(output.find("^") != std::string::npos);
  CHECK(output.find("Unexpected token") != std::string::npos);
  CHECK(output.find("let x = 42;") != std::string::npos);
}

TEST_CASE("Multi-character span produces correct tildes") {
  std::string source = "let x = 42;\nlet y = 10;";
  std::string path = "test.symph";
  File file(source, path);

  Span span(file, 4, 5);
  Diagnostic diag(diagnostic::Kind::ExpectedExpression, span,
                  "Invalid assignment");

  std::string output = diag.print();

  CHECK(output.find("^~~~") != std::string::npos); // 1 caret, 4 tildes
  CHECK(output.find("Invalid assignment") != std::string::npos);
}

TEST_CASE("Span at end of file without newline") {
  std::string source = "let a = 1;";
  std::string path = "test.symph";
  File file(source, path);

  Span span(file, 8, 2);
  Diagnostic diag(diagnostic::Kind::UnexpectToken, span, "Expected semicolon");

  std::string output = diag.print();

  CHECK(output.find("let a = 1;") != std::string::npos);
  CHECK(output.find("^~") != std::string::npos);
  CHECK(output.find("Expected semicolon") != std::string::npos);
}

TEST_CASE("Span on second line works correctly") {
  std::string source = "let x = 42;\nlet y = 10;\nlet z = 99;";
  std::string path = "test.symph";
  File file(source, path);

  Span span(file, 13, 3);
  Diagnostic diag(diagnostic::Kind::InvalidString, span, "Unused variable");

  std::string output = diag.print();

  CHECK(output.find("let y = 10;") != std::string::npos);
  CHECK(output.find("^~~") != std::string::npos);
  CHECK(output.find("Unused variable") != std::string::npos);
}