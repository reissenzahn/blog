#include <stdio.h>

int main() {
  // pointer arithmetic should only be used with arrays (and not structures) because arrays are guaranteed to be allocated in a contiguous block of memory
  int arr[5] = {1, 2, 3, 4};

  // adding an integer to a pointer adds the product of the integer and the number of bytes of the underlying data type
  int *ptr1 = arr;

  printf("%p\n", (void *) (ptr1 + 0));  // 0x7ffc9f8aa800
  printf("%p\n", (void *) (ptr1 + 1));  // 0x7ffc9f8aa804
  printf("%p\n", (void *) (ptr1 + 2));  // 0x7ffc9f8aa808
  printf("%p\n", (void *) (ptr1 + 3));  // 0x7ffc9f8aa80c

  printf("%d\n", *(ptr1 + 0));  // 1
  printf("%d\n", *(ptr1 + 1));  // 2
  printf("%d\n", *(ptr1 + 2));  // 3
  printf("%d\n", *(ptr1 + 3));  // 4

  // accessing memory past the end of an array is a dangerous
  // printf("%d\n", *(ptr1 + 4));

  // we can even use a char * to point to the bytes of an int to access the individual bytes of the integer
  int i = 0x04030201;
  char *ptr2 = &i;  // warning

  printf("%p\n", (void *) (ptr2 + 0));  // 0x7ffc9f8aa7d8
  printf("%p\n", (void *) (ptr2 + 1));  // 0x7ffc9f8aa7d9
  printf("%p\n", (void *) (ptr2 + 2));  // 0x7ffc9f8aa7da
  printf("%p\n", (void *) (ptr2 + 3));  // 0x7ffc9f8aa7db

  printf("%d\n", *(ptr2 + 0));  // 1
  printf("%d\n", *(ptr2 + 1));  // 2
  printf("%d\n", *(ptr2 + 2));  // 3
  printf("%d\n", *(ptr2 + 3));  // 4


  // subtracting an integer from is similar
  int *ptr3 = arr;
  ptr3 += 3;

  printf("%p\n", (void *) (ptr3 - 0));  // 0x7ffc9f8aa80c
  printf("%p\n", (void *) (ptr3 - 1));  // 0x7ffc9f8aa808
  printf("%p\n", (void *) (ptr3 - 2));  // 0x7ffc9f8aa804
  printf("%p\n", (void *) (ptr3 - 3));  // 0x7ffc9f8aa800

  printf("%d\n", *(ptr3 - 0));  // 4
  printf("%d\n", *(ptr3 - 1));  // 3
  printf("%d\n", *(ptr3 - 2));  // 2
  printf("%d\n", *(ptr3 - 3));  // 1


  // pointer arithmetic is closely related to array subscripting
  int *ptr4 = arr;
  int n = 2;

  printf("%p\n", (void *) &arr[n]);     // 0x7ffc9f8aa808
  printf("%p\n", (void *) (ptr4 + n));  // 0x7ffc9f8aa808
  printf("%p\n", (void *) &ptr4[n]);    // 0x7ffc9f8aa808
  printf("%p\n", (void *) &n[ptr4]);    // 0x7ffc9f8aa808

  printf("%d\n", arr[n]);       // 3
  printf("%d\n", *(ptr4 + n));  // 3
  printf("%d\n", ptr4[n]);      // 3
  printf("%d\n", n[ptr4]);      // 3

  // pointers can also be compared which can be useful to determine the relative ordering of array elements
  int *ptr5 = arr + 1;
  int *ptr6 = arr + 3;
  
  printf("%d\n", ptr5 < ptr6);  // 1

  // notice that we cannot perform pointer arithmetic on a void pointer as the compiler does not know the size of the object being pointed to

  return 0;
}