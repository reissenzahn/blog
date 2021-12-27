#include <stdio.h>

struct box {
  void *data;
};

// a function parameter can be specified as a void pointer to keep the function generic
void insert(struct box *b, void *data) {
  b->data = data;
}

// if we want to return some data via an argument we need a double void pointer
void delete(struct box *b, void **data) {
  *data = b->data;
  b->data = NULL;
}

int main(void) {
  // a void pointer is a general-purpose pointer used to hold references to any data type
  void *ptr1;
  printf("%zu\n", sizeof(void *));  // 8


  // any pointer can assigned to a void pointer without an explicit cast
  int i = 1;
  int *ptr2 = &i;
  ptr1 = ptr2;

  // though an explict case can help readability
  ptr1 = (void *) ptr2;
  printf("%p\n", (void *) ptr1);  // 0x7ffd6e5aeb9c
  printf("%p\n", (void *) ptr2);  // 0x7ffd6e5aeb9c

  // a void pointer can then be cast back to its original pointer type
  int *ptr3 = (int *) ptr1;
  printf("%d\n", *ptr3);  // 1

  // however we cannot dereference a void pointer as the compiler has no information about what is being pointed to and so no way to know how many bytes to access
  // printf("%d\n", *ptr1);  // error


  // notice once again that we do not need to explicitly cast a pointer to a void pointer
  struct box b;
  int j = 2;
  insert(&b, &j);
  printf("%d\n", *((int *) b.data));  //

  // a void pointer is compatible with any other type of pointer but a pointer to a void pointer is not so this cast is required
  int *ptr4;
  delete(&b, (void **) &ptr4);
  printf("%d\n", *ptr4);  // 2

  return 0;
}