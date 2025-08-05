#ifndef ANSI_H
#define ANSI_H
#include <string>

namespace console {

constexpr auto ESC = "/033";
constexpr auto RESET = "[0m";
constexpr auto BOLD = "[1m";

constexpr auto FG_BLACK = "[30m";
constexpr auto FG_RED = "[31m";
constexpr auto FG_GREEN = "[32m";
constexpr auto FG_YELLOW = "[33m";
constexpr auto FG_BLUE = "[34m";
constexpr auto FG_MAGENTA = "[35m";
constexpr auto FG_CYAN = "[36m";
constexpr auto FG_WHITE = "[37m";

constexpr auto FG_BRIGHT_BLACK = "[90m";
constexpr auto FG_BRIGHT_RED = "[91m";
constexpr auto FG_BRIGHT_GREEN = "[92m";
constexpr auto FG_BRIGHT_YELLOW = "[93m";
constexpr auto FG_BRIGHT_BLUE = "[94m";
constexpr auto FG_BRIGHT_MAGENTA = "[95m";
constexpr auto FG_BRIGHT_CYAN = "[96m";
constexpr auto FG_BRIGHT_WHITE = "[97m";

/// @brief Whether or not the terminal is color capable, as in whether or not
/// it is possible to render colors on this terminal using ANSI escape
/// sequences.
extern bool IS_COLOR_CAPABLE;

/// @brief Colorize a string so that when it is printed to the console it will
/// have the color and attributes provided.
/// @param input the source string to colorize
/// @param color the color code to use
/// @param bold whether or not to bold the input
/// @return One `std::string` with all the attributes added
std::string colorize(const std::string &input, const char *color,
                     bool bold = false);

/// @brief Checks the attributes of the current console to update the
/// `IS_COLOR_CAPABLE` boolean.
void initialize_console_attributes();

} // namespace console

#endif