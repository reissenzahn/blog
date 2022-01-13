#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  // strings are sequences of characters terminated with the ascii NUL character

  // we can use a suitable array literal to initialize a char array
  char str[] = {'h', 'e', 'l', 'l', 'o', '\0'};
  printf("%s\n", str);  // hello

  // the strcpy function can be used to copy the characters of a string literal into a char array
  strcpy(str, "hello");  // hello
  printf("%s\n", str);


  // when used to initialize a char[] a string literal provides a shortcut to initialize the elements of the array
  char arr[] = "hello";

  // this is a shortcut for
  // char arr[] = {'h', 'e', 'l', 'l', 'o', '\0' };

  printf("%zu\n", sizeof(arr));  // 6
  printf("%zu\n", strlen(arr));  // 5

  // everywhere else a string literal generates an unnamed array of characters with static storage that results in undefined behavior if modified
  char *ptr = "hello";

  // this is roughly similar
  // static char __unnamed[] = "hello";
  // char *ptr = __unnamed;

  // strlen can be used to get the length of a string in both cases
  printf("%zu\n", sizeof(ptr));  // 8
  printf("%zu\n", strlen(ptr));  // 5


  // it is never necessary to multiply by sizeof(char) since this is by definition exactly 1
  char *str2 = (char *) malloc(strlen("hello") + 1);
  strcpy(str2, "hello");
  printf("%s\n", str2);

  // common escape sequences in strings
  printf("\\");  // backslash
  printf("\n");  // newline
  printf("\t");  // tab
  printf("\b");  // backspace
  printf("\"");  // double quote

  return 0;
}