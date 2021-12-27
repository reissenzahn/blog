#include <stdio.h>

int main() {
  // the const qualifier declares a variable whose value cannot be changed
  const int i = 1;
  // i = 2;  // error

  // the order of the data type and the const keyword does not matter
  // int const i = 1;

  // an object qualified with const is a run-time object which cannot be assigned to but not not a constant expression in the full sense of the term which means it cannot be used for array dimensions, case labels and the like
  const int n = 5;
  int a[n];

  // however constants can be modified through pointers
  int *ptr1 = &i;
  *ptr1 = 2;  // warning
  printf("%d\n", i);  // 2

  // a pointer to a constant cannot be dereferenced to modify the data pointed to by the pointer (but can be assigned to different constant or non-constant variables)
  int j = 2;
  const int *ptr2 = &i;
  // *ptr2 = 2;  // error
  ptr2 = &j;  // ok

  // a constant pointer cannot be assigned to point to a different variable (but can be dereferenced to modify the data pointed to by the pointer)
  int k = 3;
  int * const ptr3 = &j;
  *ptr3 = 2;  // ok
  // ptr3 = &k  // error

  // a constant pointer to a constant combines the above
  const int * const ptr4 = &j;

  return 0;
}