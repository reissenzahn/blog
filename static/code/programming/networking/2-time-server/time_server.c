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
#include <time.h>

#define PORT "8080"
#define BACKLOG 10

int main() {
  int s;
  struct addrinfo hints, *bind_address;
  int socket_listen, socket_client, bytes_received, bytes_sent;
  struct sockaddr_storage client_address;
  socklen_t client_len;
  char address_buffer[100], request[1024], *now;
  time_t t;

  printf("configuring local address...\n");
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;        // tpv4 address
  hints.ai_socktype = SOCK_STREAM;  // tcp
  hints.ai_flags = AI_PASSIVE;      // bind to wildcard

  s = getaddrinfo(0, PORT, &hints, &bind_address);
  if (s != 0) {
    fprintf(stderr, "getaddrinfo: %s", gai_strerror(s));
  }

  printf("creating socket...\n");
  socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
  if (socket_listen == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  printf("binding to local address...\n");
  if (bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen) == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  freeaddrinfo(bind_address);

  printf("listening for connections...\n");
  if (listen(socket_listen, BACKLOG) == -1) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  client_len = sizeof(client_address);
  socket_client = accept(socket_listen, (struct sockaddr *) &client_address, &client_len);
  if (socket_client == -1) {
    perror("accept");
    exit(EXIT_FAILURE);
  }

  printf("handling connection...\n");
  s = getnameinfo((struct sockaddr *) &client_address, client_len, address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
  if (s != 0) {
    fprintf(stderr, "getaddrinfo: %s", gai_strerror(s));
  }

  printf("reading request from %s...\n", address_buffer);
  bytes_received = recv(socket_client, request, 1024, 0);
  if (bytes_received == -1) {
    perror("recv");
    exit(EXIT_FAILURE);
  }
  
  printf("sending response...\n");
  const char *response =
    "HTTP/1.1 200 OK\r\n"
    "Connection: close\r\n"
    "Content-Type: text/plain\r\n\r\n";
  bytes_sent = send(socket_client, response, strlen(response), 0);
  if (bytes_sent == -1) {
    perror("send");
    exit(EXIT_FAILURE);
  }

  t = time(NULL);
  now = ctime(&t);
  bytes_sent = send(socket_client, now, strlen(now), 0);
  if (bytes_sent == -1) {
    perror("send");
    exit(EXIT_FAILURE);
  }

  printf("closing...\n");
  close(socket_client);
  close(socket_listen);

  printf("done!\n");

  exit(EXIT_SUCCESS);
}

// $ gcc time_server.c && ./a.out