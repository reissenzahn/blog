#include <stdio.h>
#include <stddef.h>

// a pointer is a variable that contains the address of a memory location

int main() {
  int i = 1;

  // a pointer is typically declared with a type corresponding to what it points to though nothing inherent in a pointer indicates what type of data the pointer is referencing as it only contains an address
  int *ptr1;

  // an uninitialized pointer will contain garbage and should not be used

  // be careful when declaring multiple pointers on a single line as ptr2 is an int here
  // int *ptr1, ptr2;

  // here both ptr1 and ptr2 are pointers
  // int *ptr1, *ptr2;

  // compare this to declaring multiple pointres using a typdef
  // typedef int* PINT;
  // PINT ptr1, ptr2;

  // the size of a pointer depends on the machine in use and the compiler though on modern systems the size of a pointer to data is normally the same regardless of the pointer type
  printf("%p\n", sizeof(char *));    // 
  printf("%p\n", sizeof(int *));     // 
  printf("%p\n", sizeof(double *));  // 

  // the address-of operator gives the address of an object in memory
  ptr1 = &i;

  // one approach for printing pointers in a cross-platform way is to cast the pointer as a pointer to a void pointer and then use the %p format specifier
  printf("%p\n", (void *) ptr1);  // 0x7ffd77107c50

  // the indirection operator returns the value pointed to by the pointer (i.e. dereferences the pointer)
  *ptr1 = *ptr1 + 1;
  printf("%d\n", *ptr1);  // 2

  // pointers can use different levels of indirection and so a pointer may point to another pointer
  char *arr1[] = {"a", "b", "c", "d"};

  // array of pointers to pointers
  char **arr2[3];

  arr2[0] = &arr1[1];
  arr2[1] = &arr1[2];
  arr2[2] = &arr1[3;

  printf("%s\n",*arr2[1]);  // TODO


  // pointer to an array of length 5
  int (*a)[5];

  typedef int vec5[5];
  vec5 *a;

}