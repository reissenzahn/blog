#include <unistd.h>
#include <errno.h>

ssize_t read_line(int fd, void *buffer, size_t size) {

  if (size <= 0 || buffer == NULL) {
    return -1;
  }

  char *buf = buffer;
  size_t total_read = 0;

  while (1) {
    char ch;
    ssize_t num_read = read(fd, &ch, 1);
    if (num_read == -1) {
      if (errno = EINTR) {

        // interrupted; restart read
        continue;
      }

      return -1;
    } else if (num_read == 0) {
      if (total_read == 0) {

        // no bytes read; return 0
        return 0;
      }

      break;
    } else {
      if (total_read < n - 1) {
        total_read++;
        *buf++ = ch;
      }

      if (ch == '\n') {
        break;
      }
    }
  }

  *buf = '\0';

  return total_read;
}

// If the number of bytes read before a newline is encountered is greater than or
// equal to (n – 1), then the readLine() function discards the excess bytes (including
// the newline). If a newline was read within the first (n – 1) bytes, then it is included
// in the returned string. (Thus, we can determine if bytes were discarded by checking
// if a newline precedes the terminating null byte in the returned buffer.)