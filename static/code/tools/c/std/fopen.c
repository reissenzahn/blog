#include <stdio.h>
#include <stdlib.h>

// FILE *fopen(const char *filename, const char *mode);

// fopen() opens a file indicated by filename and returns a pointer to the file stream associated with that file

// mode is used to determine the file access mode which is 	null-terminated character string determining file access mode

//                   file already exists  file does not exist
// "r"	 open	       read from start	    failure to open
// "w"	 create	     destroy contents	    create new
// "a"	 append	     write to end	        create new
// "r+"	 read/write	 read from start	    error
// "w+"  read/write	 destroy contents	    create new
// "a+"  read/write	 write to end	        create new

// in update mode (+) both input and output may be performed but output cannot be followed by input without an intervening call to fflush, fseek, fsetpos or rewind and input cannot be followed by output without an intervening call to fseek, fsetpos or rewind, unless the input operation encountered end of file

// file access mode flag "x" can optionally be appended to "w" or "w+" specifiers which forces the function to fail if the file exists instead of overwriting it

// returns a pointer to the new file stream or a null pointer on error
 
int main() {  
  const char* filename = "/tmp/hello.txt";

  FILE* fp = fopen(fname, "w+");
  if(!fp) {
      perror("File opening failed");
      return EXIT_FAILURE;
  }

  fputs("Hello, world!\n", fp);
  
  rewind(fp);

  // an int is required to handle EOF rather than a char
  int c;
  while ((c = fgetc(fp)) != EOF) {
    putchar(c);
  }

  if (ferror(fp)) {
    puts("I/O error when reading");
  } else if (feof(fp)) {
    puts("End of file reached successfully");
    fclose(fp);
    remove(fname);
    return EXIT_SUCCESS;
  }

  fclose(fp);
  remove(fname);

  return EXIT_SUCCESS;
}