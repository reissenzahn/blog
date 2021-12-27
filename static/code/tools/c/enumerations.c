#include <stdio.h>
#include <stdlib.h>

// an enumerated type is a type whose value is a value of its underlying type which includes the values of explicitly named enumeration constants
enum color {
  RED,
  BLUE,
  GREEN
};

enum letter { A, B, C = 10, D, E = 1, F, G = F + C };

enum day {
  MONDAY,
  TUESDAY,
  WEDNESDAY,
  THURSDAY = 5,
  FRIDAY = 7,
  SATURDAY,
  SUNDAY
};

// notice that a struct or union does not introduce scope
struct element {
  int n;
  enum state { SOLID, LIQUID, GAS, PLASMA } s;
};

int main(void) {
  // each enumeration constant becomes an integer constant with type int in the enclosing scope and can be used whenever integer constants are required
  enum color r = RED;

  // the value of the first enumeration constant is zero and thereafter the value of each enumeration constant is one more than the previous enumeration constant unless the value is explicitly set
  printf("%d\n", A);  // 0
  printf("%d\n", B);  // 1
  printf("%d\n", C);  // 10
  printf("%d\n", D);  // 11
  printf("%d\n", E);  // 1
  printf("%d\n", F);  // 2
  printf("%d\n", G);  // 12

  // enumerations do not introduce new scope
  enum day d = MONDAY;

  switch (d) {
    case SATURDAY:
    case SUNDAY:
      printf("weekend!");
      break;
    default:
      printf("weekday");
  }

  struct element oxygen;
  oxygen.n = 8;
  oxygen.s = GAS;

  return 0;
}