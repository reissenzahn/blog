#include <string.h>
#include <stdio.h>

// size_t strlen(const char *str);

// returns the number of characters in a character array whose first element is pointed to by str up to and not including the first null character

// the behavior is undefined if str is not a pointer to a null-terminated byte string
 
int main(void) {
  char str[] = "Hello, world!";
 
  printf("%zu\n", strlen(str));  // 13
  printf("%zu\n", sizeof(str));  // 14
  
  str[5] = '\0';

  printf("%zu\n", strlen(str));  // 5
  printf("%zu\n", sizeof(str));  // 14
}