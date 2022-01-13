#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

// void* memcpy(void *dest, const void *src, size_t count);

// Copies count characters from the object pointed to by src to the object pointed to by dest. Both objects are interpreted as arrays of unsigned char.

// The behavior is undefined if access occurs beyond the end of the dest array. If the objects overlap, the behavior is undefined. The behavior is undefined if either dest or src is an invalid or null pointer.

// Returns a copy of dest.

int main() {

  char source[] = "once upon a midnight dreary...";
  char dest[5];

  memcpy(dest, source, sizeof(dest) - 1);
  dest[5] = '\0';
  printf("%s\n", dest);  // once

  // memcpy may be used to set the effective type of an object obtained by an allocation function.
  int *ptr = malloc(3 * sizeof(int));
  int arr[3] = {1, 2, 3};
  memcpy(ptr, arr, 3 * sizeof(int)); 

  return 0;
}