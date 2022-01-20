#include <assert.h>

// static assertions are checked at compile time while runtime assertions are checked during program execution

struct packed {
  unsigned int i;
  char *s;
};

// a static assertion is a declaration so it can appear at file scope
static_assert(sizeof(struct packed) == sizeof(unsigned int) + sizeof(char *), "struct packed must not have any padding");

int main() {
  assert(5.0 / 2 == 2.5);
  assert(5 / 2 == 2.5);
}