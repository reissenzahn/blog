#include <stdio.h>

// you can define a structure or union that contains anonymous structures or unions as fields
struct box {
  int count;

  union {
    int x;
    float y;
  };
};

int main() {

  struct box b;

  // fields in the unnamed structure or union can then be referenced as if they were declared in the containing structure or union
  b.x = 1;
  b.y = 2.0;
}

// https://gcc.gnu.org/onlinedocs/gcc/Unnamed-Fields.html