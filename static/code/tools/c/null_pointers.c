#include <stdio.h>

void inspect(int *ptr) {
  if (ptr != NULL) {
    printf("%d\n", *ptr);
  } else {
    printf("null pointer\n");
  }
}

int main(int argc, char *argv[]) {

  // for each pointer type their is a special value called the null pointer which is guaranteed to compare unequal to a pointer to any object or function
  int *ptr1 = 0;

  // zero is overloaded and refers to the null pointer as well as the integer zero
  int i = 1;
  int *ptr2 = &i;
  *ptr2 = 0;          // integer zero
  ptr2 = 0;           // null pointer

  // the NULL macro provides a convenient name for the null pointer and is typically defined as 0 or ((void *) 0)
  int *ptr3 = NULL;

  // two null pointers are always equal
  printf("%d\n", ptr1 == ptr3);  // 1


  // a null pointer is known not to point to any object or function whereas an uninitialized pointer might point anywhere
  int *ptr4;
  printf("%d\n", *ptr4);  // -98693133


  // an if statement can be used to test for non-null or null pointers
  int *ptr5 = NULL;

  if (!ptr5) {
    printf("null pointer\n");
  }

  ptr5 = &i;

  if (ptr5) {
    printf("non-null pointer\n");
  }


  // if the usage of 0 or NULL is an argument in a function call then we should cast it to the pointer type expected by the function as the compiler may not be able to tell that an unadorned 0 means a null pointer
  inspect((int *) 0);     // null pointer
  inspect((int *) NULL);  // null pointer
}