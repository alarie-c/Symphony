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
  auto line_text =
      console::colorize(std::string(span.get_line()), console::FG_MAGENTA);

  result += get_diagnostic_severity_string(severity) + " ";
  result += span.file.path + ":" + std::to_string(line) + ":" +
            std::to_string(col) + " -> ";
  result += get_diagnostic_kind_string(kind) + "\n";
  result += " | \n";
  result += " | " + line_text + "\n";

  std::string prefix_whitespace = std::string(col == 0 ? 0 : col - 1, ' ');
  std::string suffix_tildes = std::string(span.length - 1, '~');
  result +=
      " | " + prefix_whitespace +
      console::colorize(std::string("^") + suffix_tildes, console::FG_GREEN) +
      "\n";
  result += console::colorize("Help: ", console::BOLD_YELLOW) + message;

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