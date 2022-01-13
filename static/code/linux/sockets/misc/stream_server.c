#define _BSD_SOURCE // to get definitions of NI_MAXHOST and NI_MAXSERV from <netdb.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

#define PORT_NUM "50000"
#define INT_LEN 30
#define BACKLOG 50

ssize_t readline(int fd, void *buffer, size_t n) {
  if (n <= 0 || buffer == NULL) {
    errno = EINVAL;
    return -1;
  }

  // no pointer arithmetic is allowed on void *
  char *buf = buffer;
  size_t totalBytesRead = 0;

  while (1) {
    char ch;
    ssize_t bytesRead = read(fd, &ch, 1);
    
    if (bytesRead == -1) {
      if (errno == EINTR) {
        continue;
      } else {
        return -1;
      }
    }

    // handle EOF
    if (bytesRead == 0) {
      if (totalBytesRead == 0) {
        return 0;
      }

      break;
    }

    // discard > n - 1 bytes
    if (totalBytesRead < n - 1) {
      totalBytesRead++;
      *buf++ = ch;
    }

    if (ch == '\n') {
      break;
    }
  }

  *buf = '\0';

  return totalBytesRead;
}

// TODO 1221


