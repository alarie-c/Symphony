#include "common/diagnostic.hpp"
#include "common/ansi.hpp"
#include "common/span.hpp"
#include <cassert>
#include <iostream>
#include <string>

using namespace diagnostic;

Diagnostic::Diagnostic(const Kind kind, const Span span,
                       const std::string message)
    : severity(get_diagnostic_kind_severity(kind)), kind(kind), span(span),
      message(std::move(message)) {}

std::string diagnostic::Diagnostic::print() const {
  assert(span.length >= 1 && "Invalid diagnostic length");

  std::string result = "";
  auto col = span.get_column_number();
  auto line = span.get_line_number();

  result += get_diagnostic_severity_string(severity) + " ";
  result += span.file.path + ":" + std::to_string(line) + ":" +
            std::to_string(col) + " -> ";
  result += get_diagnostic_kind_string(kind) + "\n";
  result += " | \n";
  result += " | " + std::string(span.get_line()) + "\n";

  std::string prefix_whitespace = std::string(col == 0 ? 0 : col - 1, ' ');
  std::string suffix_tildes = std::string(span.length - 1, '~');
  result += " | " + prefix_whitespace + "^" + suffix_tildes + "\n";
  result += "Help: " + message;

  return result;
}

std::ostream &diagnostic::operator<<(std::ostream &os, const Kind &kind) {
  os << get_diagnostic_kind_string(kind);
  return os;
}

std::ostream &diagnostic::operator<<(std::ostream &os,
                                     const Severity &severity) {
  os << get_diagnostic_severity_string(severity);
  return os;
}