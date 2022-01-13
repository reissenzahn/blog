#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

int main(int argc, char *argv[]) {

  // signed integers
  signed char c = 1;  // at least 8 bits
  short s = 2;        // at least 16 bits
  int i = 3;          // at least 16 bits
  long l = 4;         // at least 32 bits
  long long ll = 5;   // at least 64 bits
  printf("%d %hd %d %ld %lld\n", c, s, i, l, ll);  // 1 2 3 4 5

  // unsigned integers
  unsigned char uc = 1;
  unsigned short us = 2;
  unsigned int ui = 3;
  unsigned long ul = 4;
  unsigned long long ull = 5;
  printf("%u %hu %u %lu %llu\n", uc, us, ui, ul, ull);  // 1 2 3 4 5

  // single and double-precision floating point
  float f = 3.14159;
  double d = 2.71828;
  printf("%f %lf\n", f, d);  // 3.141590 2.718280

  // a character constant represents an integer value equal to the numerical value of the character in the ascii charset
  char h = '#';
  printf("%c\n", h);  // #

  // boolean
  bool t = true;
  printf("%s", t ? "true" : "false");


  // explicitly sized integer types
  printf("%ld\n", sizeof(int8_t));   // 1
  printf("%ld\n", sizeof(int16_t));  // 2
  printf("%ld\n", sizeof(int32_t));  // 4
  printf("%ld\n", sizeof(int64_t));  // 8
  
  printf("%ld\n", sizeof(uint8_t));   // 1
  printf("%ld\n", sizeof(uint16_t));  // 2
  printf("%ld\n", sizeof(uint32_t));  // 4
  printf("%ld\n", sizeof(uint64_t));  // 8


  // size_t is the result type of the sizeof operator and represents the maximum size of any object
  size_t size = sizeof(int);
  printf("%zu\n", size);
  
  // size_t is used to store a size or index in an array
  const size_t N = 2;
  int numbers[N];
  
  for (size_t i = 0; i < N; i++) {
    printf("%d\n", numbers[i]);
  }

  // ssize_t is the sized variant of size_t that is capable of holding -1 to represent an error
  
  return 0;
}