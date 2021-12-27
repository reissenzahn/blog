#include <stdio.h>

// typedef creates an alias for an existing type.
typedef int count;


// this can be used to avoid needing to refer to a struct with the struct keyword
struct point {
  int x;
  int y;
};

typedef struct point point;

// frequently this is written in one line
// typedef struct point {
//   int x;
//   int y;
// } point;

// sometimes the name of the struct is prefixed with an underscore as a naming convention
// typedef struct _point {
//   int x;
//   int y;
// } point;

// an anonymous structure can also be used
// typedef struct {
//   int x;
//   int y;
// } point;


// typedefs are typically preferred to a #define for user-defined types in part because they can correctly encode pointer types
#define string1 char *

typedef char *string2;


// structures can contain pointers to themselves but using the name declared by a typedef before it is defined
// typedef struct {
//   char *item;
//   NODEPTR next;
// } *NODEPTR;

typedef struct node {
  char *item;
  struct node *next;
} *NODEPTR;


int main() {
  count c = 1;
  printf("%d\n", c);  // 1

  struct point p = { 1, 2 };
  printf("%d\n", p.x);  // 1

  point q = { 3, 4 };
  printf("%d\n", q.y);  // 4


  // notice that str2 is declared as char but str4 is declared as char *
  string1 str1, str2;
  string2 str3, str4;

  return 0;
}