#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_ALLOCS 1000000

int main(int argc, char *argv[]) {
  int numAllocs = 1000;
  int blockSize = 10240;

  // program break remains at its high-water mark
  // int freeStep = 1;
  // int freeMin = 1;
  // int freeMax = 999;

  // program break decreases from its peak value
  int freeStep = 1;
  int freeMin = 500;
  int freeMax = 1000;
  
  printf("Initial program break: %10p\n", sbrk(0));

  printf("Allocating %d*%d bytes\n", numAllocs, blockSize);

  char *ptr[MAX_ALLOCS];
  for (int i = 0; i < numAllocs; i++) {
    ptr[i] = malloc(blockSize);
    if (ptr[i] == NULL) {
      perror("malloc");
      exit(EXIT_SUCCESS);
    }
  }

  printf("Program break is now: %10p\n", sbrk(0));

  printf("Freeing blocks from %d to %d in steps of %d\n", freeMin, freeMax, freeStep);

  for (int i = freeMin - 1; i < freeMax; i += freeStep) {
    free(ptr[i]);
  }

  printf("After free(), program break is: %10p\n", sbrk(0));

  exit(EXIT_SUCCESS);
}