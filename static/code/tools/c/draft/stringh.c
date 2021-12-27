#include <string.h>
#include <stdio.h>

int main(void) {

  // // string manipulation

  // // char *strcpy(char *dest, const char *src);
  // // strcpy copies the null-terminated byte string pointed to by src (including the null terminator) to the character array whose first element is pointed to by dest
  // char *src = "hello";
  // char dest[strlen(src) + 1];
  // strcpy(dest, src);
  // printf("\s\n", dest);

  // // char *strncpy(char *dest, const char *src, size_t count);
  // // strncpy copies at most count characters of the character array pointed to by src (including the terminating null character but not any of the characters that follow the null character) to character array pointed to by dest
  
  // // if count is reached before the entire array src was copied then the resulting character array is not null-terminated
  // printf("\nstrncpy(dest2, src, 2) to a 2-byte dst gives : ");
  // char dest2[2];
  // strncpy(dest2, src, 2);
  // for (size_t n = 0; n < sizeof dest2; ++n) {
  //     char c = dest2[n];
  //     c ? printf("'%c' ", c) : printf("'\\0' ");
  // }
  // printf("\n");
  
  // // if count is not reached after copying the terminating null character from src then additional null characters are written to dest until the total of count characters have been written
  // char src[] = "hi";
  // char dest[6] = "abcdef"; // no null terminator
  // strncpy(dest, src, 5); // writes five characters 'h', 'i', '\0', '\0', '\0' to dest
  // printf("strncpy(dest, src, 5) to a 6-byte dest gives : ");
  //   for (size_t n = 0; n < sizeof dest; ++n) {
  //       char c = dest[n];
  //       c ? printf("'%c' ", c) : printf("'\\0' ");
  //   }
 







  // // char *strdup(const char *src);
  // // strdup returns a pointer to a null-terminated byte string which is a duplicate of the string pointed to by src that must be passed to free to avoid a memory leak
  // const char *s1 = "abcde";
  // char *s2 = strdup(s1);
  // printf("%s\n", s2);  // abcde
  // free(s2);

  // // 






  // character array manipulation
  


  // int memcmp(const void* lhs, const void* rhs, size_t count);
  // memcmp compares the first count characters of the objects pointed to by lhs and rhs; the sign of the result is the sign of the difference between the values of the first pair of bytes (both interpreted as unsigned char) that differ in the objects being compared
  char arr1[] = {'a','b','c'};
  char arr2[] = {'a','b','d'};
  printf("%d\n", memcmp(arr1, arr2, sizeof(arr1)));  // -1
  printf("%d\n", memcmp(arr2, arr1, sizeof(arr1)));  // 1

  // void *memset( void *dest, int ch, size_t count );
  // memset copies the value ch (after conversion to unsigned char as if by (unsigned char)ch) into each of the first count characters of the object pointed to by dest
  char arr3[] = "abcde";
  memset(arr3, '#', 3);
  printf("%s\n", arr3);  // ###de

  // void* memcpy(void *dest, const void *src, size_t count);
  // memcpy copies count characters from the object pointed to by src to the object pointed to by dest; both objects are interpreted as arrays of unsigned char
  char arr4[] = 

  return 0; 
}
