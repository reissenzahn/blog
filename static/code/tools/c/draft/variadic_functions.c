#include <stdio.h>
#include <stdarg.h>
  
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
  int total = sum(3, 1, 2, 3);
  printf("%d\n", total);  // 6
}