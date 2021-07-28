#include <stdio.h>

int main() {
  struct point {
    int x;
    int *y;
  };

  struct point p;

  // sizeof yields the size in bytes of the object representation of a type or the an expression
  printf("%zu\n", sizeof(struct point));  // 16
  printf("%zu\n", sizeof(p));             // 16

  // sizeof primitive types
  printf("%zu\n", sizeof(char));    // 1
  printf("%zu\n", sizeof(short));   // 2
  printf("%zu\n", sizeof(int));     // 4
  printf("%zu\n", sizeof(long));    // 8
  printf("%zu\n", sizeof(float));   // 4
  printf("%zu\n", sizeof(double));  // 8
}