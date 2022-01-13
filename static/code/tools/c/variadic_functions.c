#include <stdio.h>
#include <stdarg.h>

// a variadic function can have an argument to which a variable number of arguments can be passed

int sum(int count, ...) {	
  int total = 0;
  va_list args;

  va_start(args, count);

  for (int i = 0; i < count; i++) {
    int temp = va_arg(args, int);
    total += temp;
  }

  va_end(args);

  return total;
}

int main() {
  printf("%d\n", sum(0);            // 0
  printf("%d\n", sum(3, 1, 2, 3));  // 6
}