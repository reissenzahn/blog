#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT     8989
#define BUFSIZE  4096
#define BACKLOG  100
#define POOLSIZE 20

pthread_t threadpool[POOLSIZE];


void *handle_connection(void *arg) {
  int client_socket = *((int *) arg);
  free(arg);

  char buffer[BUFSIZE];
  int msgsize = 0;

  size_t bytes_read;
  while ((bytes_read = read(client_socket, buffer + msgsize, sizeof(buffer) - msgsize - 1)) > 0) {
    msgsize += bytes_read;

    if (msgsize > BUFSIZE - 1 || buffer[msgsize - 1] == '\n') {
      break;
    }
  }

  if (bytes_read == -1) {
    fprintf(stderr, "read(): %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  buffer[msgsize - 1] = 0;

  printf("request: %s\n", buffer);
  fflush(stdout);

  char path[PATH_MAX + 1];
  if (realpath(buffer, path) == NULL) {
    fprintf(stderr, "realpath(): %s\n", strerror(errno));
    close(client_socket);
    return NULL;
  }

  FILE *fp = fopen(path, "r");
  if (fp == NULL) {
    fprintf(stderr, "fopen(): %s\n", strerror(errno));
    close(client_socket);
    return NULL;
  }

  while ((bytes_read = fread(buffer, 1, BUFSIZE, fp) > 0)) {
    printf("sending %zu bytes\n", bytes_read);
    write(client_socket, buffer, bytes_read);
  }

  close(client_socket);
  fclose(fp);
  printf("closing connection\n");
}

int main(int argc, char *argv[]) {

  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    fprintf(stderr, "socket(): %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  if (bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
    fprintf(stderr, "bind(): %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  if (listen(server_socket, BACKLOG)) == -1) {
    fprintf(stderr, "listen(): %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  while (TRUE) {
    printf("listening...\n");

    int addr_size = sizeof(struct sockaddr_in);

    int client_socket = accept(server_socket, (struct sockaddr *) &client_addr, (socklen_t *) &addr_size);
    if (client_socket == -1) {
      fprintf(stderr, "accept(): %s\n", strerror(errno));
      exit(EXIT_FAILURE);
    }

    int *arg = malloc(sizeof(int));
    *arg = client_socket);

    pthread_t thread;
    pthread_create(&thread, NULL, handle_connection, arg);
  }

  exit(EXIT_SUCCESS);
}

// https://www.youtube.com/watch?v=Pg_4Jz8ZIH4
