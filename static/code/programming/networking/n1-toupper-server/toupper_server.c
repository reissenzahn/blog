#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define BACKLOG 10
#define BUFSIZE 1024

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "usage: %s port\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;        // ipv4
  hints.ai_socktype = SOCK_STREAM;  // tcp
  hints.ai_flags = AI_PASSIVE;      // passive bind
  hints.ai_protocol = 0;

  struct addrinfo *bind_address;
  int s = getaddrinfo(0, argv[1], &hints, &bind_address);
  if (s != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
    exit(EXIT_FAILURE);
  }

  char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
  s = getnameinfo(bind_address->ai_addr, bind_address->ai_addrlen, hbuf, NI_MAXHOST, sbuf, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
  if (s != 0) {
    fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));
    exit(EXIT_FAILURE);
  }

  int sock_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
  if (sock_listen == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  s = bind(sock_listen, bind_address->ai_addr, bind_address->ai_addrlen);
  if (s == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }
  
  freeaddrinfo(bind_address);

  s = listen(sock_listen, BACKLOG);
  if (s == -1) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  fd_set master;
  FD_ZERO(&master);
  FD_SET(sock_listen, &master);
  int max_sock = sock_listen;

  printf("Listening at %s:%s...\n", hbuf, sbuf);

  while (1) {
    fd_set reads;
    reads = master;

    s = select(max_sock + 1, &reads, 0, 0, NULL);
    if (s == -1) {
      perror("select");
      exit(EXIT_FAILURE);
    }

    for (int i = 1; i <= max_sock; i++) {
      if (FD_ISSET(i, &reads)) {
        if (i == sock_listen) {
          struct sockaddr_storage client_addr;
          socklen_t client_len = sizeof(client_addr);
          int sock_client = accept(sock_listen, (struct sockaddr *) &client_addr, &client_len);
          if (sock_client == -1) {
            perror("accept");
            exit(EXIT_FAILURE);
          }

          FD_SET(sock_client, &master);
          if (sock_client > max_sock) {
            max_sock = sock_client;
          }

          char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
          s = getnameinfo((struct sockaddr *) &client_addr, client_len, hbuf, NI_MAXHOST, sbuf, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
          if (s != 0) {
            fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));
            exit(EXIT_FAILURE);
          }

          printf("Accepted connection from %s:%s...\n", hbuf, sbuf);
        } else {
          char buf[BUFSIZE];
          int bytes = recv(i, buf, BUFSIZE, 0);
          if (bytes < 1) {
            FD_CLR(i, &master);
            close(i);
            continue;
          }

          printf("Received: %.*s (%d bytes)", bytes - 1, buf, bytes);

          for (int j = 0; j < bytes; j++) {
            buf[j] = toupper(buf[j]);
          }

          int bytes_sent = send(i, buf, bytes, 0);

          if (bytes_sent > 1) {
            printf("Sent: %.*s (%d bytes)\n", bytes, buf, bytes);
          }
        }
      }
    }
  }

  close(sock_listen);

  printf("Finished.\n");

  exit(EXIT_SUCCESS);
}

