#include "common/ansi.hpp"
#include "common/diagnostic.hpp"
#include "common/span.hpp"
#include <iostream>

int main() {
  console::initialize_console_attributes();

  std::string source = "let x = 42;\nlet y = 10;";
  std::string path = "test.symph";
  File file(source, path);

  Span span(file, 4, 5);
  Diagnostic diag(diagnostic::Kind::ExpectedExpression, span,
                  "Invalid assignment");

  std::cout << diag.print() << std::endl;
  return 0;
}