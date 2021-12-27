#include <stdio.h>

// static global variables and functions are only visible within their translation unit (i.e. the .c file after header files have been included)
static int i = 0;

static void inc1() {
  i++;
}

// a static local variable is persistent across function calls
void inc2() {
  static int j = 0;
  
  j++;
  printf("%d\n", j);
}

// a fixed-size static array can be returned from a function though each call to this function will reuse the array and invalidate any previous calls
int *allocate(int value) {
  static int arr[5];

  for (int i = 0; i < 5; i++) {
    arr[i] = value;
  }

  return arr;
}

int main() {
  inc1();
  inc1();
  inc1();
  printf("%d\n", i);  // 3

  inc2();  // 1
  inc2();  // 2
  inc2();  // 3


  int *arr1 = allocate(1);
  int *arr2 = allocate(2);

  for (int i = 0; i < 5; i++) {
    printf("%d ", arr1[i]);  // 2 2 2 2 2
  }
  printf("\n");

  return 0;
}