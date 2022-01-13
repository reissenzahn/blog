#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#define PORT    "8080"
#define BUFSIZE 1024

int main(int argc, char *argv[]) {

  // configure local address suitable for passive bind
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_protocol = 0;

  struct addrinfo *bind_address;
  int rc = getaddrinfo(0, PORT, &hints, &bind_address);
  if (rc != 0) {
    printf("getaddrinfo: %s\n", gai_strerror(rc));
    exit(EXIT_FAILURE);
  }

  // create socket suitable for local address
  int sockfd = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
  if (sockfd == -1) {
    fprintf(stderr, "socket: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  // bind socket to local address
  if (bind(sockfd, bind_address->ai_addr, bind_address->ai_addrlen) != 0) {
    fprintf(stderr, "bind: %s\n", strerror(errno));
  }
  freeaddrinfo(bind_address);

  while (true) {
    // read datagram from listening socket
    struct sockaddr_storage client_address;
    socklen_t client_len = sizeof(client_address);
    char buffer[BUFSIZE];
    int n = recvfrom(sockfd, buffer, BUFSIZE, 0, (struct sockaddr *) &client_address, &client_len);
    if (n < 1) {
      fprintf(stderr, "recvfrom: %s\n", strerror(errno));
      exit(EXIT_FAILURE);
    }

    // resolve source address and port number
    char address_buffer[NI_MAXHOST];
    char service_buffer[NI_MAXSERV];
    int rc = getnameinfo(((struct sockaddr *) &client_address), client_len, address_buffer, NI_MAXHOST, service_buffer, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
    if (rc != 0) {
      fprintf(stderr, "getnameinfo: %s\n", gai_strerror(rc));
      printf("received %d bytes from unknown:\n%.*s", n, n, buffer);
    } else {
      printf("received %d bytes from %s@%s:\n%.*s", n, address_buffer, service_buffer, n, buffer);
    }

    // send back received data in upper case
    for (int i = 0; i < n; i++) {
      buffer[i] = toupper(buffer[i]);
    }

    sendto(sockfd, buffer, n, 0, (struct sockaddr *) &client_address, client_len);
  }

  close(sockfd);

  exit(EXIT_SUCCESS);
}