// event_loop.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>

#define MAXEVENTS 64

static void make_non_blocking(int sfd) {
  int flags, s;

  flags = fcntl(sfd, F_GETFL);
  if (flags == -1) {
    perror("fcntl");
    exit(EXIT_FAILURE);
  }

  flags |= O_NONBLOCK;

  s = fcntl(sfd, F_SETFL, flags);
  if (s == -1) {
    perror("fcntl");
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char *argv[]) {
  struct addrinfo hints;
  struct addrinfo *results, *result;
  int s, sfd, flags, efd;
  struct epoll_event event;
  struct epoll_event *events;

  if (argc != 2) {
    fprintf(stderr, "usage: %s [port]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // find address suitable for passive bind
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;      // ipv4 or ipv6
  hints.ai_socktype = SOCK_STREAM;  // tcp socket
  hints.ai_flags = AI_PASSIVE;      // suitable for passive open

  s = getaddrinfo(NULL, argv[1], &hints, &results);
  if (s != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
    exit(EXIT_FAILURE);
  }

  // loop over address candidates until successful bind
  for (result = results; result != NULL; result = result->ai_next) {
    sfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (sfd == -1) {
      continue;
    }

    s = bind(sfd, result->ai_addr, result->ai_addrlen);
    if (s == -1) {
      close(sfd);
      continue;
    }

    break;
  }

  freeaddrinfo(results);

  if (result == NULL) {
    fprintf(stderr, "failed to bind to address\n");
    exit(EXIT_FAILURE);
  }

  // make socket non-blocking
  make_non_blocking(sfd);

  // start listening
  s = listen(sfd, SOMAXCONN);
  if (s == -1) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  // create epoll instance
  efd = epoll_create(1);
  if (efd == -1) {
    perror("epoll_create");
    exit(EXIT_FAILURE);
  }

  // add listening socket to interest list
  event.data.fd = sfd;
  event.events = EPOLLIN | EPOLLET;  // edge-triggered

  s = epoll_ctl(efd, EPOLL_CTL_ADD, sfd, &event);
  if (s == -1) {
    perror("epoll_ctl");
    exit(EXIT_FAILURE);
  }

  // initialize buffer to hold returned events
  events = calloc(MAXEVENTS, sizeof(event));

  // enter event loop
  while (1) {
    int n, i;

    n = epoll_wait(efd, events, MAXEVENTS, -1);

    for (i = 0; i < n; i++) {

      // check if error occurred or socket is not ready
      if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || !(events[i].events & EPOLLIN)) {
        fprintf(stderr, "epoll error occurred\n");
        close(events[i].data.fd);
        continue;
      }

      // handle incoming connections on listening socket
      if (sfd == events[i].data.fd) {
        while (1) {
          struct sockaddr in_addr;
          socklen_t in_len;
          int infd;
          char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

          in_len = sizeof(in_addr);

          // accept incoming connection          
          infd = accept(sfd, &in_addr, &in_len);
          if (infd == -1) {
            if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
              break;
            }

            perror("accept");
            break;
          }

          // display address of remote peer if possible
          s = getnameinfo(&in_addr, in_len, hbuf, sizeof(hbuf), sbuf, sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV);
          if (s == 0) {
            printf("accepted connection from %s:%s on fd %d\n", hbuf, sbuf, infd);
          }

          // make the remote peer socket non-blocking
          make_non_blocking(infd);

          // add remote peer socket to interest list
          event.data.fd = infd;
          event.events = EPOLLIN | EPOLLET;

          s = epoll_ctl(efd, EPOLL_CTL_ADD, infd, &event);
          if (s == -1) {
            perror("epoll_ctl");
            exit(EXIT_FAILURE);
          }
        }

        continue;
      }

      // handle data waiting to be read
      int done = 0;

      while (1) {
        ssize_t count;
        char buf[512];

        count = read(events[i].data.fd, buf, sizeof(buf));
        if (count == -1) {
          if (errno != EAGAIN) {
            perror("read");
            done = 1;
          }

          break;
        } else if (count == 0) {
          done = 1;
          break;
        }

        // write to standard out
        s = write(STDOUT_FILENO, buf, count);
        if (s == -1) {
          perror("write");
          exit(EXIT_FAILURE);
        }
      }

      if (done) {
        printf("closing connection on fd %d\n", events[i].data.fd);
        close(events[i].data.fd);
      }
    }
  }

  free(events);
  close(sfd);

  return EXIT_SUCCESS;
}

// $ gcc event_loop.c
// $ ./a.out
// $ telnet localhost 8000
// > hello!