#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

// #include <limits.h>
// #include <stdlib.h>

// char *realpath(const char *path, char *resolved_path);

// realpath() expands all symbolic links and resolves references to /./, /../ and extra / characters in the null-terminated string named by path to produce a canonicalized absolute pathname.

// The resulting pathname is stored as a null-terminated string, up to a maximum of PATH_MAX bytes, in the buffer pointed to by resolved_path.

// If resolved_path is specified as NULL then realpath() uses malloc() to allocate a buffer of up to PATH_MAX bytes to hold the resolved pathname and returns a pointer to this buffer. The caller should  deallocate this buffer using free(3).

// If there is no error, realpath() returns a pointer to the resolved_path. Otherwise, it returns  NULL, the contents of the array resolved_path are undefined and errno is set to indicate the error.


int main(int argc, char *argv[]) {

  if (argc != 2) {
    fprintf(stderr, "usage: %s path\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char *path = realpath(argv[1], NULL);

  printf("%s\n", path);

  exit(EXIT_SUCCESS);
}
