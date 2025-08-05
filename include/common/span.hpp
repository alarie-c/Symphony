#ifndef SPAN_H
#define SPAN_H
#include <iostream>
#include <string>

struct File {
  std::string content;
  std::string path;
  size_t length;

  File(std::string content, std::string path);
};

struct Span {
  const File &file;
  size_t offset;
  size_t length;

  Span(const File &file, size_t offset, size_t length);
  friend std::ostream &operator<<(std::ostream &os, const Span &span);

  std::string stringify() const;
  std::string_view get_lexeme() const;
  std::string_view get_line() const;
  int get_column_number() const;
  int get_line_number() const;
};

#endif