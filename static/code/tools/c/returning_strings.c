#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// when we return the address of a string we need to ensure that address remains valid

// we can return a reference to a literal
char *f1() {
  return "hello!";
}

// we can return a reference to statically declared literal
char *f2() {
  static char *str = "hello!";
  return str;
}

// returning a pointer to a static string used for multiple purposes can be a problem
char *f3(char *who) {
  static char str[64];  // assume large enough
  sprintf(str, "hello, %s!", who);
  return str;
}

// dynamically allocated memory will also work though the caller must be sure to free this memory
char *f4() {
  char *str = (char *) malloc(strlen("hello!") + 1);
  strcpy(str, "hello!");
  return str;
}

// we can allow the caller to provide a buffer in which to store the string
char *f5(char *str, size_t len) {
  if (str == NULL) {
    len = strlen("hello!") + 1;
    str = (char *) malloc(len);
  }

  strncpy(str, "hello!", len);
  str[len - 1] = '\0';  // make sure we terminate

  // we typically return a pointer to the buffer
  return str;
}

// returning a local string variable on the stack should be avoided as the memory will be overwritten by another stack frame
// char *f6(bool casual) {
//   char str[] = "hello!";
//   return str;
// }

int main(int argc, char *argv[]) {
  printf("%s\n", f1());  // hello!

  printf("%s\n", f2());  // hello!

  char *str1 = f3("steven");
  char *str2 = f3("alex");
  printf("%s\n", str1);  // hello, alex!
  printf("%s\n", str2);  // hello, alex!

  char *str3 = f4();
  printf("%s\n", str3);  // hello!
  free(str3);

  char str4[10];
  f5(str4, sizeof(str4));
  printf("%s\n", str4);  // hello!

  printf("%s\n", f5(NULL, 0));  // hello!

  return 0;
}