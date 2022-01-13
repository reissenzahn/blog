#include <stdio.h>
#include <stdlib.h>

// returning a pointer to local data is dangerous as the address of the array returned is no longer valid once the function returns and its stack frame is popped off the stack and can be overwritten by subsequent function calls
int *allocate1(int size, int value) {
  int arr[size];

  for (int i = 0; i < size; i++) {
    arr[i] = value;
  }

  return arr;
}

// instead we can dynamically allocate memory within the function and return the address of that memory while making the caller responsible for deallocating the memory
int *allocate2(int size, int value) {
  int *arr = (int *) malloc(size * sizeof(int));

  if (arr != NULL) {
    for (int i = 0; i < size; i++) {
      arr[i] = value;
    }
  }

  return arr;
}

// we can also pass an object to the function for it to be modified thus making the caller responsible for both allocation and deallocation
int *allocate3(int *arr, int size, int value) {
  if (arr != NULL) {
    for (int i = 0; i < size; i++) {
      arr[i] = value;
    }
  }

  return arr;
}

// as all parameters (including pointers) are passed by value we need to pass a pointer to a pointer if we want to modify the original pointer and not the copy pointer
void allocate4(int **arr, int size, int value) {
  *arr = (int *) malloc(size * sizeof(int));

  if (*arr != NULL) {
    for (int i = 0; i < size; i++) {
      *(*arr + i) = value;
    }
  }
}

int main() {  
  int *arr1 = allocate1(5, 1);
  // printf("%d\n", arr1[2]);  // segfault

  int *arr2 = allocate2(5, 2);
  printf("%d\n", arr2[2]);  // 2
  free(arr2);

  int arr3[5];
  allocate3(arr3, 5, 3);
  printf("%d\n", arr3[2]);  // 3

  int *arr4 = NULL;
  allocate4(&arr4, 5, 4);
  printf("%d\n", arr4[2]);  // 4

  return 0;
}