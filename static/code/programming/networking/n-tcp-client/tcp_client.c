// $ gcc tcp_client.c && ./a.out example.com 80
// GET / HTTP/1.1
// Host: example.com
// 

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

#define BUFSIZE 4096

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "usage: %s hostname port\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;         // ipv4 or ipv6
  hints.ai_socktype = SOCK_STREAM;  // tcp
  hints.ai_flags = 0;
  hints.ai_protocol = 0;

  struct addrinfo *addr;
  int s = getaddrinfo(argv[1], argv[2], &hints, &addr);
  if (s != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
    exit(EXIT_FAILURE);
  }

  char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
  s = getnameinfo(addr->ai_addr, addr->ai_addrlen, hbuf, NI_MAXHOST, sbuf, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
  if (s != 0) {
    fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));
    exit(EXIT_FAILURE);
  }
  
  int sock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
  if (sock == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  printf("Connecting to %s:%s... ", hbuf, sbuf);
  
  s = connect(sock, addr->ai_addr, addr->ai_addrlen);
  if (s == -1) {
    perror("connect");
    exit(EXIT_FAILURE);
  }

  freeaddrinfo(addr);

  printf("done!\n\n");

  printf("Enter text followed by enter to send:\n");

  while (1) {
    fd_set reads;
    FD_ZERO(&reads);
    FD_SET(sock, &reads);
    FD_SET(STDIN_FILENO, &reads);

    s = select(sock + 1, &reads, 0, 0, NULL);
    if (s == -1) {
      perror("select");
      exit(EXIT_FAILURE);
    }

    if (FD_ISSET(sock, &reads)) {
      char buf[BUFSIZE];
      int bytes = recv(sock, buf, BUFSIZE, 0);
      if (bytes < 1) {
        printf("Disconnected!");
        break;
      }

      printf("Received: %.*s (%d bytes)\n", bytes, buf, bytes);
    }

    if (FD_ISSET(STDIN_FILENO, &reads)) {
      char buf[BUFSIZE];
      if (fgets(buf, BUFSIZE, stdin) == NULL) {
        break;
      }

      int bytes = send(sock, buf, strlen(buf) - 1, 0);
      
      if (bytes > 1) {
        printf("Sent: %.*s (%d bytes)\n", bytes, buf, bytes);
      }
    }
  }

  close(sock);

  printf("Finished.\n");

  exit(EXIT_SUCCESS);
}