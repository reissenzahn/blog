#include <stdio.h>

int main() {
  // the result of integer division is truncated towards zero
  printf("%d\n", 5 / 2);   // 2
  printf("%d\n", -5 / 2);  // -2

  // this can be addressed by casting one of the operands to a floating-point type or by using a floating-point constant
  printf("%f\n", (double) 5 / 2);  // 2.5
  printf("%f\n", -5 / 2.0);        // -2.5


  // i++ increments i and returns the un-incremented value while ++i increments i and returns the incrmented value
  int i = 1;
  printf("%d\n", i++);  // 1

  int j = 1;
  printf("%d\n", ++j);  // 2


  // bitwise operators
  unsigned char x = 5;  // 00000101
  unsigned char y = 9;  // 00001001

  printf("%d\n", x & y);   // 00000001
  printf("%d\n", x | y);   // 00001101
  printf("%d\n", x ^ y);   // 00001100
  printf("%d\n", ~x);      // 11111010
  printf("%d\n", y << 2);  // 00100100
  printf("%d\n", y >> 2);  // 00000010


  // be aware of overflows or underflows that can occur in arithmetic expresions
  int a = 100000, b = 100000;
  long c = a * b;
  long long d = (long) a * b;
  printf("%d\n", c);   // 1410065408
  printf("%ld\n", d);  // 10000000000

  return 0;
}