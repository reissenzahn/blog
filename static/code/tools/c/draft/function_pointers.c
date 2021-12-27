#include <stdio.h>

// a function pointer is a pointer that holds the address of a function

int add(int n, int m) {
  return n + m;
}

int sub(int n, int m) {
  return n - m;
}

// a function pointer can be passed as an argument to a function
int apply1(int (*fptr)(int, int), int n, int m) {
  return fptr(n, m);
}

// a function pointer can also be returned from a function
int (*select1(char c))(int, int) {
  switch(c) {
  case '+':
    return add;
  case '-':
    return sub;
  }
}

// it is often convenient to declare a type defintion for function pointers
typedef int (*operation)(int, int);

int apply2(operation fptr, int n, int m) {
  return fptr(n, m);
}

operation select2(char c) {
  switch(c) {
  case '+':
    return add;
  case '-':
    return sub;
  }
}

int main() {
  // declare a pointer that accepts to integers as arguments and returns an integer
  int (*fptr1)(int, int);
  
  // assign the address of a function to a function pointer and invoke it by means of the pointer
  fptr1 = add;
  printf("%d\n", fptr1(1, 2));  // 3

  // use of the address-of operator and indirection operators is redundant
  fptr1 = &add;
  printf("%d\n", (*fptr1)(1, 2));  // 3


  // a function pointer can be passed as an argument to a function 
  operation fptr2 = sub;
  printf("%d\n", apply1(fptr2, 1, 2));  // 1
  printf("%d\n", apply2(sub, 1, 2));    // 1

  // a function pointer can also be returned from a function
  printf("%d\n", apply1(select1('+'), 1, 2));  // 3
  printf("%d\n", apply1(select2('+'), 1, 2));  // 3


  // function pointers can be compared for equality
  int (*fptr3)(int, int) = add;
  printf("%d\n", fptr3 == add);  // 1
  printf("%d\n", fptr3 != sub);  // 1


  // we can create an array of function pointers
  int (*arr1[128])(int, int) = {NULL};
  arr1['+'] = add;
  arr1['-'] = sub;

  operation arr2[128] = {NULL};
  arr2['+'] = add;
  arr2['-'] = sub;

  printf("%d\n", arr1['+'](1, 2));  // 3
  printf("%d\n", arr2['-'](1, 2));  // -1
}