#include <stdio.h>
#include <stdlib.h>

// a #define can be used to define function-like behavior
#define SQUARE(x) x * x


// a multi-statement macro must be defined carefully without a trailing semicolon
#define COUNT(to) do { \
  for (int i = 1; i <= to; i++) { \
    printf("%d ", i); \
  } \
  printf("\n"); \
} while(0)


// there are three rules to follow when defining function-like macros:

// 1. the macro expansion must always be parenthesized to protect any lower-precedence operators from the surrounding expression (e.g. with 1 / SQUARE(n) we get 1 / n * n instead of 1 / (n * n))

// 2. in the macro definition all occurrences of the parameters must be parenthesized to protect any low-precedence operators in the actual arguments from the rest of the macro expansion (e.g. with SQUARE(n + 1) we get n + 1 * n + 1 instead of (n + 1) * (n + 1))

// 3. if a parameter appears several times in the expansion then the macro may not work properly if the actual argument is an expression with side effects (e.g. SQUARE(i++) would expand to i++ * i++ which is undefined)

// accounting for rule 3 can be difficult so we might just mark the macro as unsafe to remember not to use it on arguments with side effects
#define CUBE(x) ((x) * (x) * (x))


// using temporary variables with types specified by the typeof operator can be useful to handle arguments that are expressions with side effects (notice that the typeof operator does not evaluate the expression)
#define MIN(A, B) ((A) < (B) ? (A) : (B))

#define MAX(A, B) ({ \
  typeof(A) _a = (A); \
  typeof(B) _b = (B); \
  _a > _b ? _a : _b; \
})




int main() {

  printf("%d\n", SQUARE(2));  // 4
  printf("%d\n", CUBE(2));    // 8


  int i = 1, j = 1;
  printf("%d\n", MIN(++i, 1000));  // 3
  printf("%d\n", MAX(++j, 0));     // 2

  COUNT(5);  // 1 2 3 4 5

  return 0;
}

// $ gcc -E preprocessor_functions.c
// $ gcc preprocessor_functions.c && ./a.out