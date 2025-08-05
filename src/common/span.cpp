#include "common/span.h"

File::File(std::string content, std::string path)
    : content(content), path(path), length(content.size()) {}

Span::Span(const File &file, size_t offset, size_t length)
    : file(file), offset(offset), length(length) {}

std::ostream &operator<<(std::ostream &os, const Span &span) {
  int line = span.get_line_number();
  int col = span.get_column_number();

  os << span.file.path << ":" << line << ":" << col;
  return os;
}

std::string_view Span::get_lexeme() const {
  size_t len = length;
  if (offset + len > file.length)
    len = file.length - offset;

  return std::string_view(file.content).substr(offset, len);
}

int Span::get_column_number() const {
  const std::string &content = file.content;

  // Find the newline before the offset
  auto line_start = content.rfind('\n', offset == 0 ? 0 : offset - 1);

  // The newline before the file doesn't exist
  // becase this offset is on the first line
  if (line_start == std::string::npos)
    return static_cast<int>(offset + 1);

  return static_cast<int>(offset - line_start);
}

int Span::get_line_number() const {
  const std::string &content = file.content;
  int line = 1;

  for (size_t i = 0; i < offset && i < file.length; i++) {
    if (content[i] == '\n') {
      line++;
    }
  }

  return line;
}