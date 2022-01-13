#include <stdio.h>

// parameters are passed by value and so a copy of the arguments are passed to the function when it is called
void zero(int i) {
  i = 0;
}

// passing a pointer allows the function to modify the data referenced by the pointer and avoids the overhead of copying that data
void swap(int *i, int *j) {
  int tmp = *i;
  *i = *j;
  *j = tmp;
}

// we can pass data by pointer and prohibit it from being modified by passing it as a pointer to a constant while also avoiding copy overhead
void copy(const int *i, int *j) {
  *j = *i;
}

int main() {
  int i = 1;
  zero(i);
  printf("%d\n", i);  // 1

  int j = 2;
  swap(&i, &j);
  printf("%d\n", i);  // 2
  printf("%d\n", j);  // 1

  copy(&i, &j);
  printf("%d\n", i);  // 2
  printf("%d\n", j);  // 2
}