#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define ADDRESS "127.0.0.1"
#define PORT    "8080"
#define BUFSIZE 1024

int main(int argc, char *argv[]) {

  // configure remote server address
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_protocol = 0;

  struct addrinfo *remote_address;
  int rc = getaddrinfo(ADDRESS, PORT, &hints, &remote_address);
  if (rc != 0) {
    printf("getaddrinfo: %s\n", gai_strerror(rc));
    exit(EXIT_FAILURE);
  }

  // resolve and display remote address
  char address_buffer[NI_MAXHOST];
  char service_buffer[NI_MAXSERV];
  rc = getnameinfo(remote_address->ai_addr, remote_address->ai_addrlen, address_buffer, NI_MAXHOST, service_buffer, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
  if (rc != 0) {
    fprintf(stderr, "getnameinfo: %s\n", gai_strerror(rc));
  } else {
    printf("sending to %s@%s\n", address_buffer, service_buffer);
  }

  // create socket suitable for remote address
  int sockfd = socket(remote_address->ai_family, remote_address->ai_socktype, remote_address->ai_protocol);
  if (sockfd == -1) {
    fprintf(stderr, "socket: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  // TODO: use select to read from stdin and handle received data
  while (true) {

    // read input from standard input
    char buffer[BUFSIZE];
    int bytes_read = read(STDIN_FILENO, buffer, BUFSIZE);
    if (bytes_read == -1) {
      fprintf(stderr, "read: %s\n", strerror(errno));
      exit(EXIT_FAILURE);
    }

    int bytes_sent = sendto(sockfd, buffer, bytes_read, 0, remote_address->ai_addr, remote_address->ai_addrlen);

    printf("sent %d bytes\n", bytes_sent);

    // TODO: rename client_address
    struct sockaddr_storage client_address;
    socklen_t client_len = sizeof(client_address);
    char response_buffer[BUFSIZE];
    int n = recvfrom(sockfd, response_buffer, BUFSIZE, 0, (struct sockaddr *) &client_address, &client_len);
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
      printf("received %d bytes from unknown:\n%.*s", n, n, response_buffer);
    } else {
      printf("received %d bytes from %s@%s:\n%.*s", n, address_buffer, service_buffer, n, response_buffer);
    }
  }

  freeaddrinfo(remote_address);

  close(sockfd);

  exit(EXIT_SUCCESS);
}
