#include <stdio.h>

int main(int argc, const char* argv[]) {
  printf("Hello, C!\n");

  for (int i = 0; i < argc; i++) {
    printf("%s\n", argv[i]);
  }

  return 0;
}