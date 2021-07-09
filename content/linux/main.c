#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <string.h>

int main() {
  // create a new epoll instance
  int epfd = epoll_create1(0);
  if (epfd == -1) {
    perror("epoll_create1");
    return 1;
  }

  struct epoll_event event;
  event.events = EPOLLIN;        // input events
  event.data.fd = STDIN_FILENO;  // stdin

  // add stdin to the epoll instance
  if (epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event)) {
    perror("epoll_ctl");
    close(epfd);
    return 1;
  };

  while (go) {
    
  }

  // close epoll instance
  if (close(epfd)) {
    perror("close");
    return 1;
  }

  return 0;
}