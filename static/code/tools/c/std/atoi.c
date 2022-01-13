#include <stdlib.h>
#include <stdio.h>

// int atoi(const char *str);
// long atol(const char *str);
// long long atoll(const char *str);

// interprets an integer value in a byte string pointed to by str

// discards any whitespace characters until the first non-whitespace character is found then takes as many numeric digits as possible to form a valid integer number representation and converts them to an integer value

// returns an integer value corresponding to the contents of str on success or 0 if no conversion can be performed

// if the value of the result cannot be represented (i.e. the converted value falls out of range of the corresponding return type) then behavior is undefined

int main() {
  
  printf("%d\n", atoi("   123junk"));  // 123
  printf("%d\n", atoi("123 456"));     // 123
  printf("%d\n", atoi("-123"));        // -123
  printf("%d\n", atoi("junk"));        // 0
  printf("%d\n", atoi("2147483648"));  // undefined

  return 0;
}