#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <limits.h>

#define PORT 8989
#define BUFSIZE 4096
#define BACKLOG 1

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

void handle_connection(int client_socket);
int check(int exp, const char *msg);

int main()
{
  // TODO: update to getaddrinfo etc.
  // TODO: error handling
  
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  bind(server_socket, (struct sockaddr *) *server_addr, sizeof(server_addr));

  listen(server_socket, BACKLOG);

  while (true) {
    
  }

  return EXIT_SUCCESS;
}
