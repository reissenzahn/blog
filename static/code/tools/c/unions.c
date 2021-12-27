#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// a union is a structure in which all of the fields overlay each other in memory
union basket {
  int i;
  float f;
  char s[20];
};

// to keep track of which field of a union is in use we can use a tagged union where we make sure the code field is always set appropriately when the union is written to
struct tagged {
  enum {UNKNOWN, INT, DOUBLE, POINTER} code;
  union {
    int i;
    double d;
    void *p;
  } u;
};

int main(void) {
  union basket b;

  // the size of a union is the maximum of the sizes of its individual members plus any padding
  printf("%d\n", sizeof(b));  // 20


  // you can only use one field at a time as the fields are overlayed in memory
  b.i = 1;
  printf("%d\n", b.i);  // 1

  b.f = 1.0;
  printf("%f\n", b.f);  // 1.0

  strcpy(b.s, "hello");
  printf("%s\n", b.s);  // hello


  struct tagged t;
  t.u.i = 1;
  t.code = INT;
  printf("%d\n", t.u.i);  // 1

  return 0;
}