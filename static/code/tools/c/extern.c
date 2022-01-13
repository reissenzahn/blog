#include <stdio.h>

// an external variable must be defined (exactly once) outside of any function
int i = 0;

// external variables can be accessed by name by any function
void inc() {
  // an external variable must also be declared in each function that wants to it
  extern int i;

  i++;
}


// function declarations have an implcit extern applied so failing to provide a definition causes a linker error instead of a compiler error
int add(int a, int b);

// function definitions are also implicitly extern
int sub(int a, int b) {
  return a - b;
}


int main() {
  // if the definition of the external variable occurs in the same source file then the extern declaration can be omitted

  inc();
  inc();
  inc();
  printf("%d\n", i);  // 3
}