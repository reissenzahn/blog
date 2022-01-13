#include <stdio.h>
#include <stdlib.h>

// dynamic memory management involves manually allocating and deallocating memory on the heap

int main() {
  // malloc() allocates a specified number of bytes and returns a void pointer to the allocated memory on the heap
  char *ptr1 = (char *) malloc(6 * sizeof(char));

  // a null pointer indicates an error occurred when allocating memory
  if (ptr1 == NULL) {
    printf("failed to allocate memory!\n");
    return 1;
  }

  for (int i = 0; i < 5; i++) {
    *(ptr1 + i) = 'a' + i;
  }

  // there is no portable way to determine the size of the memory allocated by a call to malloc()
  printf("%zu\n", (void *) ptr1);  // size of pointer

  // we must be careful to avoid writing outside the bounds of the allocated memory
  *(ptr1 + 5) = '\0';

  printf("%s\n", ptr1);  // abcde

  // free() deallocates allocate memory
  free(ptr1);

  // a pointer should not be accessed (or freed again) after it is freed and so a common practice is to always assign NULL to a freed pointer
  ptr1 = NULL;


  // calloc() dynamically allocates zero-initialized memory for a specified number of elements of a specified size
  int *ptr2 = (int *) calloc(5, sizeof(int));

  // this is essentially equivalent to
  // int *ptr2 = malloc(5 * sizeof(int));
  // memset(ptr2, 0, 5 * sizeof(int));

  if (ptr2 == NULL) {
    printf("failed to allocate memory!\n");
    return 1;
  }

  // the zero-initialized memory is guaranteed to yield the value 0 for all integral types (including '\0' for character types) but it does not guarantee useful null pointer values or floating-point zero values
  for (int i = 0; i < 5; i++) {
    printf("%d ", ptr2[i]);  // 0 0 0 0 0
  }
  printf("\n");

  for (int i = 0; i < 5; i++) {
    ptr2[i] = i;
  }

  for (int i = 0; i < 5; i++) {
    printf("%d ", ptr2[i]);  // 0 1 2 3 4
  }
  printf("\n");

  free(ptr2);


  // realloc() resizes a block of memory to a larger or smaller amount based on a previously allocated block of memory and returns a pointer to the reallocated memory
  double *ptr3 = (double *) malloc(5 * sizeof(double));

  if (ptr3 == NULL) {
    printf("failed to allocate memory!\n");
    return 1;
  }

  for (int i = 0; i < 5; i++) {
    ptr3[i] = (double) i;
  }

  // if the specified size is zero and the pointer is not null then the pointer will be freed
  double *ptr4 = (double *) realloc(ptr3, 10 * sizeof(double));

  // if the realloccation fails then the original block of memory is retained unchanged and a null pointer is returned
  if (ptr4 == NULL) {
    printf("failed to allocate memory!\n");
    free(ptr3);
    return 1;
  }

  for (int i = 5; i < 10; i++) {
    ptr4[i] = i;
  }

  for (int i = 0; i < 10; i++) {
    printf("%.0f ", ptr4[i]);  // 0 1 2 3 4 5 6 7 8 9
  }
  printf("\n");

  free(ptr4);

  // if the pointer is NULL then the call is equivalent to malloc() with the specified size


  // when a process terminates all allocated memory is returned to the operating system
  return 0;
}


// Every object has an effective type, which determines which lvalue accesses are valid and which violate the strict aliasing rules.

// If the object was created by a declaration, the declared type of that object is the object's effective type.

// If the object was created by an allocation function (including realloc), it has no declared type. Such object acquires an effective type as follows:

// The first write to that object through an lvalue that has a type other than character type, at which time the type of that lvalue becomes this object's effective type for that write and all subsequent reads.
// memcpy or memmove copy another object into that object, at which time the effective type of the source object (if it had one) becomes the effective type of this object for that write and all subsequent reads.
// Any other access to the object with no declared type, the effective type is the type of the lvalue used for the access.
