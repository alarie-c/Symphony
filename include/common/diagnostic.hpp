#ifndef DIAGNOSTIC_H
#define DIAGNOSTIC_H
#include "ansi.hpp"
#include "span.hpp"
#include <iostream>
#include <string>
#include <vector>

namespace diagnostic {

#define DIAGNOSTIC_SEVERITIES                                                  \
  X(Error, "Error:", console::BOLD_RED)                                        \
  X(Warning, "Warning:", console::BOLD_MAGENTA)                                \
  X(Info, "Info:", console::BOLD_BRIGHT_BLUE)

/// @brief Represents the severity of some diagnostic. Not all diagnostic kinds
/// are errors, and only errors can abort compilation.
enum class Severity {
#define X(name, str, color) name,
  DIAGNOSTIC_SEVERITIES
#undef X
};

/// @brief Returns the string representation of some diagnostic severity.
/// @param severity the severity variant to retrieve a `std::string` for
/// @return the string representation that is given by this `Severity` along
/// with the ANSI escape codes for the severity's colors
constexpr std::string get_diagnostic_severity_string(const Severity &severity) {
  using enum Severity;

  switch (severity) {
#define X(name, str, color)                                                    \
  case name:                                                                   \
    return console::colorize(str, color);
    DIAGNOSTIC_SEVERITIES
#undef X
  };
  return std::string("<unknown>");
}

/* ---------------------------------------------------------------------------*/
/* DIAGNOSTIC KINDS */
/* ---------------------------------------------------------------------------*/

#define DIAGNOSTIC_KINDS                                                       \
  X(InvalidCharacter, "Invalid Character", Severity::Error)                    \
  X(UnterminatedString, "Unterminated String", Severity::Error)                \
  X(InvalidString, "Invalid String", Severity::Error)                          \
  X(UnexpectToken, "Unexpected Token", Severity::Error)                        \
  X(ExpectedExpression, "Expected Expression", Severity::Error)                \
  X(InternalError, "Internal Error", Severity::Error)

/// @brief Represents a specific kind of error encountered by the compiler. This
/// can be from any phase during compilation.
enum class Kind {
#define X(name, str, severity) name,
  DIAGNOSTIC_KINDS
#undef X
};

/// @brief Returns the `Severity` of any given diagnostic kind.
/// @param kind the `Kind` to retrieve a severity for
/// @return some variant of `Severity` that is given by this `Kind`, which can
/// be found in the definition for `DIAGNOSTIC_KINDS`
constexpr Severity get_diagnostic_kind_severity(const Kind &kind) {
  using enum Kind;

  switch (kind) {
#define X(name, str, severity)                                                 \
  case name:                                                                   \
    return severity;
    DIAGNOSTIC_KINDS
#undef X
  };
  return Severity::Error;
}

/// @brief Returns the string representation of some diagnostic kind.
/// @param kind the kind to retrieve a `std::string` for
/// @return the string representation that is given by this `Kind`, which can
/// be found in the definition for `DIAGNOSTIC_KINDS`
constexpr std::string get_diagnostic_kind_string(const Kind &kind) {
  using enum Kind;

  switch (kind) {
#define X(name, str, severity)                                                 \
  case name:                                                                   \
    return str;
    DIAGNOSTIC_KINDS
#undef X
  };
  return std::string("<unknown>");
}

/* ---------------------------------------------------------------------------*/
/* DIAGNOSTIC */
/* ---------------------------------------------------------------------------*/

struct Diagnostic {
  const Severity severity;
  const Kind kind;
  const Span span;
  const std::string message;

  Diagnostic(const Kind kind, const Span span, const std::string message);
  std::string print() const;
};

std::ostream &operator<<(std::ostream &os, const Kind &kind);
std::ostream &operator<<(std::ostream &os, const Severity &severity);

} // namespace diagnostic

using Diagnostic = diagnostic::Diagnostic;

class DiagnosticEngine {
  std::vector<Diagnostic> diagnostics;

public:
  DiagnosticEngine();
  void push(const Diagnostic diag);
  void print_all() const;
};

#endif