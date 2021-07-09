#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <string.h>

int main() {

  // create a new epoll instance
  int epoll_fd = epoll_create1(0);
  if (epoll_fd == -1) {
    fprintf(stderr, "failed to create epoll fd\n");
    return 1;
  }


  struct epoll_event event;
  event.events = EPOLLIN;
  event.data.fd = STDIN_FILENO;

  


  if (close(epoll_fd)) {
    fprintf(stderr, "failed to close epoll fd\n");
    return 1;
  }

  return 0;
}