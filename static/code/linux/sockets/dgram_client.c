#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ctype.h>
#include <stdio.h>

#define BUF_SIZE 10
#define PORT_NUM 50002

int main(int argc, char *argv[]) {
  if (argc < 3) == 0) {
    printf("%s host-address msg...\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int sfd = socket(AF_INET6, SOCK_DGRAM, 0);
  if (sfd == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in6 svaddr;
  memset(&svaddr, 0, sizeof(struct sockaddr_in6));
  svaddr.sin6_family = AF_INET6;
  svaddr.sin6_port = htons(PORT_NUM);
  
  if (inet_pton(AF_INET6, argv[1], &svaddr.sin6_addr) <= 0) {
    perror("inet_pton");
    exit(EXIT_FAILURE);
  }

  for (int i = 2; i < argc; i++) {
    size_t msgLen = strlen(argv[i]);
    if (sendto(sfd, argv[i], msgLen, 0, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_in6)) != msgLen) {
      perror("sendto");
      exit(EXIT_FAILURE);
    }

    char buf[BUF_SIZE];
    ssize_t numBytes = recvfrom(sfd, buf, BUF_SIZE, 0, NULL, NULL);
    if (numBytes == -1) {
      perror("recvfrom");
      exit(EXIT_FAILURE);
    }

    printf("Received %.*s\n", (int numBytes), buf);
  }

  exit(EXIT_FAILURE);
}