#include <stdio.h>
#include <stddef.h>
#include <locale.h>

// int scanf(const char *format, ...);
// int fscanf(FILE *stream, const char *format, ...);
// int sscanf(const char *buffer, const char *format, ...);

// Reads data from stdin, a file stream or a null-terimanted character string buffer, interprets the data according to format and stores the results into given locations.

// Reaching the end of the string is equivalent to reaching the end-of-file condition for fscanf.

 
int main()
{
  int i, j;
  float x, y;
  char str1[10], str2[4];

  char input[] = "25 54.32E-1 Thompson 56789 0123 56";

  // %d: an integer
  // %f: a floating-point value
  // %9s: a string of at most 9 non-whitespace characters
  // %2d: two-digit integer (digits 5 and 6)
  // %f:  a floating-point value (digits 7, 8, 9)
  // %*d: an integer which isn't stored anywhere
  // ' ': all consecutive whitespace
  // %3[0-9]: a string of at most 3 decimal digits (digits 5 and 6)

  int ret = sscanf(input, "%d%f%9s%2d%f%*d %3[0-9]", &i, &x, str1, &j, &y, str2);

  printf("i = %d\nx = %f\nstr1 = %s\n j = %d\ny = %f\nstr2 = %s\n", i, x, str1, j, y, str2);


  // most conversion specifiers consume all consecutive whitespace
  int p, q;

  sscanf("1\n\t 2", "%d%d", &p, &q);

  printf("%d\n", p);  // 1
  printf("%d\n", q);  // 2

  // the conversion specifiers that do not consume leading whitespace can be made to do so by using a whitespace character in the format string
  int r;
  char c;

  sscanf("1\n\ta", "%d %c", &r, &c);

  printf("%d\n", r);
  printf("%c\n", c);

  return 0;
}