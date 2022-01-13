

int snprintf(char *buffer, size_t bufsz, const char *format, ...);

// Loads the data from the given locations, converts them to character string equivalents and writes at most bufsz - 1 characters to a string buffer.

// Returns the number of characters (not including the terminating null character) which would have been written to buffer if bufsz was ignored, or a negative value if an error occurred.

// The resulting character string will be terminated with a null character, unless bufsz is zero. If bufsz is zero, nothing is written and buffer may be a null pointer, however the return value (number of bytes that would be written not including the null terminator) is still calculated and returned.

int main() {
  const char* s = "Hello";
  // string padding
  printf("\t.%10s.\n\t.%-10s.\n\t.%*s.\n", s, s, 10, s);
  
  // string 
  printf("\t%.4s\n\t%.*s\n", s, 3, s);

  printf("Characters:\t%c %%\n", 65);

  printf("Integers\n");
  printf("Decimal:\t%i %d %.6i %i %.0i %+i %i\n", 1, 2, 3, 0, 0, 4, -4);
  printf("Hexadecimal:\t%x %x %X %#x\n", 5, 10, 10, 6);
  printf("Octal:\t\t%o %#o %#o\n", 10, 10, 4);

  printf("Floating point\n");
  printf("Rounding:\t%f %.0f %.32f\n", 1.5, 1.5, 1.3);
  printf("Padding:\t%05.2f %.2f %5.2f\n", 1.5, 1.5, 1.5);
  printf("Scientific:\t%E %e\n", 1.5, 1.5);
  printf("Hexadecimal:\t%a %A\n", 1.5, 1.5);


}

Calling snprintf with zero bufsz and null pointer for buffer is useful to determine the necessary buffer size to contain the output:

const char *fmt = "sqrt(2) = %f";
int sz = snprintf(NULL, 0, fmt, sqrt(2));
char buf[sz + 1]; // note +1 for terminating null byte
snprintf(buf, sizeof buf, fmt, sqrt(2));
snprintf_s, just like snprintf, but unlike sprintf_s, will truncate the output to fit in bufsz-1.g