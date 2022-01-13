#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// char *strdup(const char *src);

// strdup() returns a pointer to a null-terminated byte string which is a duplicate of the string pointed to by src

// the space for the new string is obtained as if malloc() was invoked and so the returned pointer must be passed to free() to avoid a memory leak

// the call returns a null pointer on error

int main(void) {
  const char *str1 = "Duplicate me!";

  char *str2 = strdup(str1);
  
  printf("%s\n", str2);
  
  free(str2);
}