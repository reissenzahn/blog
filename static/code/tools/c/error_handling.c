#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
  // usage check
  if (argc != 2 || strcmp(argv[1], "--help") == 0) {
    fprintf(stderr, "usage: %s [filename]\n", argv[0]);

    // non-zero return values signal unusual or erroneous termination conditions
    return -1;
  }

  
  // error handling is typically handled using return values where -1 is often used to indicate an error has occurred
  int fd = open(argv[1], O_RDONLY);

  if (fd == -1) {
    // print value of errno
    fprintf(stderr, "open: %i\n", errno);

    // print error message
    fprintf(stderr, "open: %s\n", strerror(errno));

    // print error message with prefix
    perror("open");

    return -1;
  }

  close(fd);


  // there are macros for obtaining the filename and line number which can be useful for debugging
  fprintf(stderr, "something went wrong in %s on line %d!\n", __FILE__, __LINE__);
  

  // using setjmp() and longjmp() 


  // a return value of zero implies normal termination
  return 0;
}