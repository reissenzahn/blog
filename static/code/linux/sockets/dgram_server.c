#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ctype.h>
#include <stdio.h>

#define BUF_SIZE 10
#define PORT_NUM 50002

int main(int argc, char *argv[]) {

  int sfd = socket(AF_INET6, SOCK_DGRAM, 0);
  if (sfd == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in6 svaddr;
  memset(&svaddr, 0, sizeof(struct sockaddr_in6));
  svaddr.sin6_family = AF_INET6;
  svaddr.sin6_addr = in6addr_any;
  svaddr.sin6_port = htons(PORT_NUM);

  if (bind(sfd, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_in6)) == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  while (1) {
    char buf[BUF_SIZE];
    struct sockaddr_in6 claddr;
    socklen_t len = sizeof(struct sockaddr_in6);
    ssize_t numBytes = recvfrom(sfd, buf, BUF_SIZE, 0, (struct sockaddr *) &claddr, &len);
    if (numBytes == -1) {
      perror("recvfrom");
      exit(EXIT_FAILURE);
    }

    char claddrStr[INET6_ADDRSTRLEN];
    if (inet_ntop(AF_INET6, &claddr.sin6_addr, claddrStr, INET6_ADDRSTRLEN) == NULL) {
      printf("Failed to convert client address to string.\n");
    } else {
      printf("Received %ld bytes from (%s, %u)\n", (long) numBytes, claddrStr, ntohs(claddr.sin6_port));
    }

    for (int i = 0; i < numBytes; i++) {
      buf[i] = toupper((unsigned char) buf[i])
    }

    if (sendto(sfd, buf, numBytes, 0, (struct sockaddr *) &claddr, len) != numBytes) {
      perror("sendto");
      exit(EXIT_FAILURE);
    }
  }
}
