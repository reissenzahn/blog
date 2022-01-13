#include <stdio.h>
#include <stdlib.h>

// ssize_t getline(char **lineptr, size_t *n, FILE *stream);

// reads from a stream until \n is encountered and stores the characters in the buffer of size *n pointed to by *lineptr and adds a null terminator

// the side of the buffer pointed to by *lineptr is automatically increased to fit the entire input (including the \n and the null pointer) and n is updated accordingly

// lineptr may be null in which case n is ignored and getline allocates a new buffer

// returns the number of characters stored in the buffer including the delimiter but excluding the null terminator or -1 on error


 getline()  reads  an entire line from stream, storing the address of the buffer
       containing the text into *lineptr.  The buffer is null-terminated and  includes
       the newline character, if one was found.

       If *lineptr is set to NULL before the call, then getline() will allocate a buf‐
       fer for storing the line.  This buffer should be freed by the user program even
       if getline() failed.

       Alternatively,  before  calling  getline(), *lineptr can contain a pointer to a
       malloc(3)-allocated buffer *n bytes in size.  If the buffer is not large enough
       to  hold  the line, getline() resizes it with realloc(3), updating *lineptr and
       *n as necessary.

       In either case, on a successful call, *lineptr and *n will be  updated  to  re‐
       flect the buffer address and allocated size respectively.

On success, getline() and getdelim() return the number of characters read,  in‐
       cluding  the  delimiter  character, but not including the terminating null byte
       ('\0').  This value can be used to handle embedded null bytes in the line read.

       Both functions return -1 on failure to read a line (including end-of-file  con‐
       dition).  In the event of a failure, errno is set to indicate the error.
  

int main() {
  char *response = NULL;
  size_t len;
    
  if ((getline(&response, &len, stdin) < 0) || (len && response[0] == 'n')) {
    free(response);
    return 0;
  }

  free(response);
  
  return 0;
}