#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "common/span.h"
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
