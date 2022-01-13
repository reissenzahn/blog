#include <stdio.h>

struct point {
  int x;
  int y;
};

int main() {
  // sizeof yields the size in bytes of the object representation of a type or expression
  int i;
  printf("%zu\n", sizeof(int));  // 4
  printf("%zu\n", sizeof(i));    // 4


  // using sizeof with an array type gives the total number of bytes of the underlying array
  int arr[5];
  printf("%zu\n", sizeof(int[5]));  // 20
  printf("%zu\n", sizeof(arr));     // 20

  // though this does not work when dealing with an array value that has decayed to a pointer
  int *ptr = arr;
  printf("%zu\n", sizeof(int *));  // 8
  printf("%zu\n", sizeof(ptr));    // 8


  // using sizeof with a structure or union type gives the total number of bytes including any padding
  struct point p;
  printf("%zu\n", sizeof(struct point));  // 8
  printf("%zu\n", sizeof(p));             // 8


  // we always have that sizeof(char) == sizeof(signed char) == sizeof(unsigned char) == 1
  printf("%zu\n", sizeof(char));           // 1
  printf("%zu\n", sizeof(signed char));    // 1
  printf("%zu\n", sizeof(unsigned char));  // 1


  // when using sizeof with strings we must remember it will include the \0 byte
  printf("%zu\n", sizeof("hello"));  // 6


  // sizeof primitive types
  printf("%zu\n", sizeof(char));    // 1
  printf("%zu\n", sizeof(short));   // 2
  printf("%zu\n", sizeof(int));     // 4
  printf("%zu\n", sizeof(long));    // 8
  printf("%zu\n", sizeof(float));   // 4
  printf("%zu\n", sizeof(double));  // 8
}

// https://stackoverflow.com/questions/37538/how-do-i-determine-the-size-of-my-array-in-c
