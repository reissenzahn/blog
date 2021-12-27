#include <stdio.h>

int main() {
  // if else
  int score = 0;
  if (!score) {
    printf("zero\n");  // zero
  } else if (score > 0) {
    printf("+\n");
  } else {
    printf("-\n");
  }


  // ternary operator
  3 >= 2 ? printf("+\n") : printf("-\n");  // +


  // switch
  char answer = 'y';
  switch (answer) {
    case 'y':
      printf("yes\n");  // yes
      break;
    case 'n':
      printf("no\n");
      break;
    default:
      printf("huh?\n");
      break;
  }


  // looping
  int i = 0;
  while (i < 5) {
    printf("%d ", i++);  // 0 1 2 3 4
  }
  printf("\n");

  for (int i = 0; i < 5; i++) {
    printf("%d ", i);  // 0 1 2 3 4
  }
  printf("\n");


  // break exits a loop while continue immediately re-evaluates the looping condition
  for (int i = 0; i < 10; i++) {
    if (i != 0 && i % 7 == 0) {
      break;
    }

    if (i % 2 == 0) {
      continue;
    }

    printf("%d ", i);  // 1 3 5
  }
  printf("\n");


  // goto causes execution to jump to a specified label
  goto skip;
  printf("here\n");
  
  // labels have function scope so you can only goto labels that are in the same function
  skip:
  printf("there\n");  // there

  // this can be used to bail out of nested loops
  while (1) {
    while (1) {
      goto bail;
    }
  }
  bail:
  printf("escaped!\n");
}