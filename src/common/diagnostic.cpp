#include "common/diagnostic.hpp"
#include "common/ansi.hpp"
#include "common/span.hpp"
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

using namespace diagnostic;

Diagnostic::Diagnostic(const Kind kind, const Span span,
                       const std::string message)
    : severity(get_diagnostic_kind_severity(kind)), kind(kind), span(span),
      message(std::move(message)) {}

std::string diagnostic::Diagnostic::print() const {
  assert(span.length >= 1 && "Invalid diagnostic length");

  auto col = span.get_column_number();
  auto line = span.get_line_number();
  auto line_text =
      console::colorize(std::string(span.get_line()), console::FG_MAGENTA);

  std::stringstream ss;

  // Build header
  ss << get_diagnostic_severity_string(severity) << " ";
  ss << span.file.path << ":" + std::to_string(line) << ":"
     << std::to_string(col) << " -> ";
  ss << get_diagnostic_kind_string(kind) << "\n";

  // Build source code representation
  ss << " | \n";
  ss << " | " << line_text << "\n";

  // Build the underline highlighter
  std::string prefix_whitespace = std::string(col == 0 ? 0 : col - 1, ' ');
  std::string suffix_tildes = std::string(span.length - 1, '~');
  ss << " | " + prefix_whitespace
     << console::colorize(std::string("^") + suffix_tildes, console::FG_GREEN)
     << "\n";

  // Build the help message
  ss << console::colorize("Help: ", console::BOLD_YELLOW) + message;

  return ss.str();
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