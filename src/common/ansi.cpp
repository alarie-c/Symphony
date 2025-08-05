#include "common/ansi.hpp"

bool console::IS_COLOR_CAPABLE = false;

#ifdef _WIN32
#include <io.h>
#include <windows.h>
void console::initialize_console_attributes() {
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  if (hOut == INVALID_HANDLE_VALUE)
    return;

  DWORD dwMode = 0;
  if (!GetConsoleMode(hOut, &dwMode))
    return;

  dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  if (!SetConsoleMode(hOut, dwMode))
    return;

  if (_isatty(_fileno(stdout)))
    console::IS_COLOR_CAPABLE = true;
}
#else
#include <unistd.h>
void console::initialize_console_attributes() {
  if (isatty(STDOUT_FILENO))
    console::IS_COLOR_CAPABLE = true;
}
#endif

std::string console::colorize(const std::string &input, const char *color) {
  using namespace console;

  // Early return the input if the terminal isn't color capable
  if (!IS_COLOR_CAPABLE)
    return std::string(input);

  // Append data to the resulting string
  std::string result = ESC;
  result += color;
  result += input;
  result += ESC;
  result += RESET;

  return result;
}