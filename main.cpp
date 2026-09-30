#include <fmt/core.h>

int main() {
  int x = 5;    // modernize/readability 可能提示
  if (x == 5) { // bugprone 会提示可以用 true
  }
  fmt::print("Hello, {}!\n", "World");
  return 0;
}